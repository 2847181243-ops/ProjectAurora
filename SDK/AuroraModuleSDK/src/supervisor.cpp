#include "aurora/supervisor.h"

#include <utility>

#include "aurora/diagnostics.h"
#include "aurora/state_machine.h"

namespace aurora {

bool isFatalStatus(AuroraStatus status) {
    switch (status) {
        case AURORA_E_INIT_FAILED:
        case AURORA_E_INTERNAL:
        case AURORA_E_ABI_MISMATCH:
        case AURORA_E_OUT_OF_MEMORY:
        case AURORA_E_STATE:
            return true;
        default:
            return false;
    }
}

ModuleSupervisor::ModuleSupervisor(ModuleRegistry& registry, ModuleHost& host)
    : registry_(registry), host_(host) {}

ModuleSupervisor::~ModuleSupervisor() { shutdownAll(); }

AuroraStatus ModuleSupervisor::start(const std::string& module_id,
                                     const AuroraGameInfo& game,
                                     std::string& error) {
    error.clear();
    ModuleEntry* entry = registry_.find(module_id);
    if (entry == nullptr) {
        error = "unknown module: " + module_id;
        return AURORA_E_INVALID_ARGUMENT;
    }
    if (instances_.count(module_id) != 0) {
        error = "module is already running: " + module_id;
        return AURORA_E_STATE;
    }
    if (entry->state != AURORA_STATE_READY) {
        error = std::string("module is ") + aurora_module_state_string(entry->state);
        if (!entry->unmet.empty()) {
            error += " (";
            for (std::size_t i = 0; i < entry->unmet.size(); ++i) {
                if (i != 0) error += ", ";
                error += entry->unmet[i];
            }
            error += ")";
        }
        return AURORA_E_STATE;
    }

    const AuroraModuleVTable* vtable = entry->vtable;
    HostBinding* binding = host_.createBinding(module_id);

    RuntimeInstance instance;
    instance.module_id = module_id;
    instance.binding = binding;
    instance.status.state = AURORA_STATE_READY;

    /* Every call into the module is wrapped. A C++ module that throws where it
     * promised not to still must not take the supervisor down with it. */
    AuroraStatus status = AURORA_OK;
    try {
        status = vtable->create_instance(&binding->services, &instance.handle);
    } catch (...) {
        status = AURORA_E_INTERNAL;
    }
    if (status != AURORA_OK || instance.handle == nullptr) {
        error = std::string("create_instance failed: ") + aurora_status_string(status);
        if (instance.handle != nullptr) {
            try {
                vtable->release_instance(instance.handle);
            } catch (...) {
                host_.diagnostics().record(AURORA_LOG_ERROR, module_id,
                                           "release_instance threw during startup cleanup");
            }
        }
        host_.releaseBinding(binding);
        return fail(module_id, status == AURORA_OK ? AURORA_E_INIT_FAILED : status, error, error);
    }

    try {
        status = vtable->load_game(instance.handle, &game);
    } catch (...) {
        status = AURORA_E_INTERNAL;
    }
    if (status != AURORA_OK) {
        error = std::string("load_game failed: ") + aurora_status_string(status);
        try {
            vtable->release_instance(instance.handle);
        } catch (...) {
            host_.diagnostics().record(AURORA_LOG_ERROR, module_id,
                                       "release_instance threw after a load failure");
        }
        host_.releaseBinding(binding);
        if (isFatalStatus(status)) return fail(module_id, status, error, error);
        /* A rejected game is a user mistake, not a module defect: stay READY. */
        registry_.evaluateAll();
        return status;
    }

    try {
        status = vtable->start(instance.handle);
    } catch (...) {
        status = AURORA_E_INTERNAL;
    }
    if (status != AURORA_OK) {
        error = std::string("start failed: ") + aurora_status_string(status);
        try {
            vtable->release_instance(instance.handle);
        } catch (...) {
            host_.diagnostics().record(AURORA_LOG_ERROR, module_id,
                                       "release_instance threw after a start failure");
        }
        host_.releaseBinding(binding);
        return fail(module_id, status, error, error);
    }

    instance.started = true;
    instance.status.state = AURORA_STATE_RUNNING;
    instances_.emplace(module_id, instance);
    applyTransition(entry->state, AURORA_STATE_RUNNING);
    host_.diagnostics().record(AURORA_LOG_INFO, module_id,
                               "session started: " + std::string(game.game_path ? game.game_path : ""));
    return AURORA_OK;
}

AuroraStatus ModuleSupervisor::tick(const std::string& module_id, double delta_seconds,
                                    std::string& error) {
    error.clear();
    auto it = instances_.find(module_id);
    if (it == instances_.end() || !it->second.started) {
        error = "module is not running: " + module_id;
        return AURORA_E_STATE;
    }
    const ModuleEntry* entry = registry_.find(module_id);
    if (entry == nullptr || entry->vtable == nullptr) {
        error = "module disappeared from the registry";
        return fail(module_id, AURORA_E_INTERNAL, error, error);
    }
    AuroraStatus (*tick)(void*, double) = aurora_vtable_tick(entry->vtable);
    if (tick == nullptr) return AURORA_OK;

    AuroraStatus status = AURORA_OK;
    try {
        status = tick(it->second.handle, delta_seconds);
    } catch (...) {
        status = AURORA_E_INTERNAL;
    }
    if (status != AURORA_OK) {
        error = std::string("tick failed: ") + aurora_status_string(status);
        return fail(module_id, status, error, error);
    }
    if (entry->vtable->query_status != nullptr) {
        AuroraModuleInstanceStatus snapshot{};
        AuroraStatus query = AURORA_OK;
        try {
            query = entry->vtable->query_status(it->second.handle, &snapshot);
        } catch (...) {
            query = AURORA_E_INTERNAL;
        }
        if (query == AURORA_OK) it->second.status = snapshot;
    }
    return AURORA_OK;
}

AuroraStatus ModuleSupervisor::pause(const std::string& module_id, std::string& error) {
    error.clear();
    auto it = instances_.find(module_id);
    if (it == instances_.end() || !it->second.started) {
        error = "module is not running: " + module_id;
        return AURORA_E_STATE;
    }
    const ModuleEntry* entry = registry_.find(module_id);
    if (entry == nullptr || entry->vtable == nullptr) {
        error = "module disappeared from the registry";
        return fail(module_id, AURORA_E_INTERNAL, error, error);
    }
    if (entry->vtable->pause == nullptr) return AURORA_E_UNSUPPORTED;
    AuroraStatus status = AURORA_OK;
    try {
        status = entry->vtable->pause(it->second.handle);
    } catch (...) {
        status = AURORA_E_INTERNAL;
    }
    if (status != AURORA_OK) {
        error = std::string("pause failed: ") + aurora_status_string(status);
        return fail(module_id, status, error, error);
    }
    it->second.status.state = AURORA_STATE_RUNNING; /* paused is a sub-state of running */
    return AURORA_OK;
}

AuroraStatus ModuleSupervisor::resume(const std::string& module_id, std::string& error) {
    error.clear();
    auto it = instances_.find(module_id);
    if (it == instances_.end() || !it->second.started) {
        error = "module is not running: " + module_id;
        return AURORA_E_STATE;
    }
    const ModuleEntry* entry = registry_.find(module_id);
    if (entry == nullptr || entry->vtable == nullptr) {
        error = "module disappeared from the registry";
        return fail(module_id, AURORA_E_INTERNAL, error, error);
    }
    if (entry->vtable->resume == nullptr) return AURORA_E_UNSUPPORTED;
    AuroraStatus status = AURORA_OK;
    try {
        status = entry->vtable->resume(it->second.handle);
    } catch (...) {
        status = AURORA_E_INTERNAL;
    }
    if (status != AURORA_OK) {
        error = std::string("resume failed: ") + aurora_status_string(status);
        return fail(module_id, status, error, error);
    }
    return AURORA_OK;
}

AuroraStatus ModuleSupervisor::stop(const std::string& module_id, std::string& error) {
    error.clear();
    auto it = instances_.find(module_id);
    if (it == instances_.end()) {
        error = "module is not running: " + module_id;
        return AURORA_E_STATE;
    }
    const ModuleEntry* entry = registry_.find(module_id);
    AuroraStatus status = AURORA_OK;
    if (entry != nullptr && entry->vtable != nullptr) {
        try {
            status = entry->vtable->stop(it->second.handle);
        } catch (...) {
            status = AURORA_E_INTERNAL;
        }
    }
    teardown(module_id);
    ModuleEntry* mutable_entry = registry_.find(module_id);
    if (mutable_entry != nullptr && mutable_entry->state == AURORA_STATE_RUNNING) {
        applyTransition(mutable_entry->state, AURORA_STATE_READY);
    }
    registry_.evaluateAll();
    if (status != AURORA_OK) {
        error = std::string("stop reported: ") + aurora_status_string(status);
        return status;
    }
    host_.diagnostics().record(AURORA_LOG_INFO, module_id, "session closed");
    return AURORA_OK;
}

void ModuleSupervisor::teardown(const std::string& module_id) {
    auto it = instances_.find(module_id);
    if (it == instances_.end()) return;
    const ModuleEntry* entry = registry_.find(module_id);
    if (entry != nullptr && entry->vtable != nullptr && it->second.handle != nullptr) {
        try {
            entry->vtable->release_instance(it->second.handle);
        } catch (...) {
            host_.diagnostics().record(AURORA_LOG_ERROR, module_id,
                                       "release_instance threw; resources may leak");
        }
    }
    if (it->second.binding != nullptr) {
        host_.releaseBinding(it->second.binding);
    }
    instances_.erase(it);
}

void ModuleSupervisor::shutdownAll() {
    std::vector<std::string> ids;
    ids.reserve(instances_.size());
    for (const auto& kv : instances_) ids.push_back(kv.first);
    for (const auto& id : ids) {
        std::string ignored;
        stop(id, ignored);
    }
}

bool ModuleSupervisor::isRunning(const std::string& module_id) const {
    auto it = instances_.find(module_id);
    return it != instances_.end() && it->second.started;
}

const RuntimeInstance* ModuleSupervisor::instance(const std::string& module_id) const {
    auto it = instances_.find(module_id);
    return it == instances_.end() ? nullptr : &it->second;
}

bool ModuleSupervisor::resetFailure(const std::string& module_id, std::string& error) {
    error.clear();
    ModuleEntry* entry = registry_.find(module_id);
    if (entry == nullptr) {
        error = "unknown module: " + module_id;
        return false;
    }
    entry->last_error.clear();
    registry_.evaluateAll();
    return true;
}

AuroraStatus ModuleSupervisor::fail(const std::string& module_id,
                                    AuroraStatus status,
                                    const std::string& message,
                                    std::string& error) {
    error = message;
    teardown(module_id);
    ModuleEntry* entry = registry_.find(module_id);
    if (entry != nullptr) {
        /* Whatever happens, the entry must not be left RUNNING with no instance:
         * start() would refuse (not READY), stop() would refuse (no instance) and
         * setEnabled() would refuse (RUNNING) - a module wedged until relaunch. */
        entry->last_error = message;
        const AuroraModuleState target =
            isFatalStatus(status) ? AURORA_STATE_FAILED : AURORA_STATE_READY;
        const std::vector<AuroraModuleState> path = transitionPath(entry->state, target);
        for (AuroraModuleState step : path) applyTransition(entry->state, step);
    }
    host_.diagnostics().record(AURORA_LOG_ERROR, module_id, message);
    return status;
}

} // namespace aurora
