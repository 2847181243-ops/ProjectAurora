/* A module that fails on purpose.
 *
 * Every failure mode in the fault-isolation spec needs a way to be reproduced on
 * demand. The trigger is the game format so the behaviour stays deterministic and
 * needs no environment variables:
 *   "ok"          behaves
 *   "unsupported" load_game returns AURORA_E_UNSUPPORTED_GAME (user error)
 *   "crash"       load_game throws C++ across the ABI (module bug, must be caught)
 *   "failstart"   start returns AURORA_E_INIT_FAILED (fatal, sticky)
 *   "stopfail"    stop fails while the platform is tearing the instance down
 */
#include <cstring>
#include <stdexcept>

#include <new>

#include "aurora/module_api.h"

namespace {

const char* kModuleId = "com.aurora.runtime.faultinjector";

struct Instance {
    const AuroraHostServices* host = nullptr;
    char mode[32] = {0};
    bool started = false;
};

void logLine(Instance* instance, AuroraLogLevel level, const char* message) {
    if (instance->host != nullptr && instance->host->write_log != nullptr) {
        instance->host->write_log(instance->host->host_context, kModuleId, level, message);
    }
}

AuroraStatus createInstance(const AuroraHostServices* host, void** out_instance) {
    if (host == nullptr || out_instance == nullptr) return AURORA_E_INVALID_ARGUMENT;
    Instance* instance = new (std::nothrow) Instance();
    if (instance == nullptr) return AURORA_E_OUT_OF_MEMORY;
    instance->host = host;
    *out_instance = instance;
    return AURORA_OK;
}

AuroraStatus loadGame(void* handle, const AuroraGameInfo* game) {
    if (handle == nullptr || game == nullptr || game->format == nullptr) return AURORA_E_INVALID_ARGUMENT;
    Instance* instance = static_cast<Instance*>(handle);
    std::strncpy(instance->mode, game->format, sizeof(instance->mode) - 1);
    instance->mode[sizeof(instance->mode) - 1] = '\0';

    if (std::strcmp(instance->mode, "unsupported") == 0) {
        logLine(instance, AURORA_LOG_WARNING, "rejecting this game on purpose");
        return AURORA_E_UNSUPPORTED_GAME;
    }
    if (std::strcmp(instance->mode, "crash") == 0) {
        logLine(instance, AURORA_LOG_ERROR, "throwing across the ABI on purpose");
        throw std::runtime_error("fault injector: exception across the C ABI");
    }
    logLine(instance, AURORA_LOG_INFO, "loaded in mode");
    return AURORA_OK;
}

AuroraStatus startInstance(void* handle) {
    Instance* instance = static_cast<Instance*>(handle);
    if (instance == nullptr) return AURORA_E_INVALID_ARGUMENT;
    if (std::strcmp(instance->mode, "failstart") == 0) {
        logLine(instance, AURORA_LOG_ERROR, "failing initialization on purpose");
        return AURORA_E_INIT_FAILED;
    }
    instance->started = true;
    return AURORA_OK;
}

AuroraStatus pauseInstance(void* handle) { return handle == nullptr ? AURORA_E_INVALID_ARGUMENT : AURORA_OK; }
AuroraStatus resumeInstance(void* handle) { return handle == nullptr ? AURORA_E_INVALID_ARGUMENT : AURORA_OK; }

AuroraStatus stopInstance(void* handle) {
    Instance* instance = static_cast<Instance*>(handle);
    if (instance == nullptr) return AURORA_E_INVALID_ARGUMENT;
    const bool poison = std::strcmp(instance->mode, "stopfail") == 0;
    instance->started = false;
    if (poison) return AURORA_E_IO;
    return AURORA_OK;
}

AuroraStatus queryStatus(void* handle, AuroraModuleInstanceStatus* out_status) {
    if (handle == nullptr || out_status == nullptr) return AURORA_E_INVALID_ARGUMENT;
    Instance* instance = static_cast<Instance*>(handle);
    out_status->state = instance->started ? AURORA_STATE_RUNNING : AURORA_STATE_BUILT;
    out_status->frames_presented = 0;
    out_status->frames_dropped = 0;
    out_status->average_fps = 0.0;
    out_status->name = kModuleId;
    return AURORA_OK;
}

void releaseInstance(void* handle) { delete static_cast<Instance*>(handle); }

const AuroraModuleVTable kTable = {
    sizeof(AuroraModuleVTable), AURORA_ABI_MAJOR, AURORA_ABI_MINOR,
    kModuleId, "1.0.0",
    &createInstance, &loadGame, &startInstance, &pauseInstance, &resumeInstance,
    &stopInstance, &queryStatus, &releaseInstance,
    nullptr,
};

} // namespace

AURORA_MODULE_ENTRY(faultinjector) {
    return &kTable;
}
