// SPDX-License-Identifier: GPL-3.0-or-later
#include <atomic>
#include <cstring>
#include <filesystem>
#include <new>
#include <string>
#include <thread>

#include "aurora/module_api.h"
#include "ons_engine.h"

namespace {

const char* kModuleId = "com.aurora.runtime.onscripter";

/* ONScripter drives its own loop, so the module is a supervisor rather than a
 * frame source: start() spawns the engine thread, tick() only observes it. */
struct Instance {
    const AuroraHostServices* host = nullptr;
    aurora::ons::Config config;
    std::thread worker;
    std::atomic<bool> running{false};
    std::atomic<int> result{0};
    std::string status_name = "onscripter";
};

void logLine(Instance* instance, AuroraLogLevel level, const std::string& message) {
    if (instance->host != nullptr && instance->host->write_log != nullptr) {
        instance->host->write_log(instance->host->host_context, kModuleId, level, message.c_str());
    }
}

std::string storagePath(Instance* instance, AuroraStorageKind kind, const char* fallback) {
    if (instance->host == nullptr || instance->host->storage_directory == nullptr) return fallback;
    char buffer[1024];
    const AuroraStatus status = instance->host->storage_directory(
        instance->host->host_context, kModuleId, kind, "", buffer, sizeof(buffer));
    return status == AURORA_OK ? std::string(buffer) : std::string(fallback);
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
    if (handle == nullptr || game == nullptr || game->game_path == nullptr) return AURORA_E_INVALID_ARGUMENT;
    Instance* instance = static_cast<Instance*>(handle);
    /* The library may hand over the script file itself (0.txt, nscript.dat).
     * ONScripter wants the directory that holds the archive. */
    std::error_code ec;
    std::filesystem::path candidate(game->game_path);
    if (std::filesystem::is_regular_file(candidate, ec)) {
        candidate = candidate.parent_path();
    }
    instance->config.game_directory = candidate.generic_string();
    instance->config.save_directory = storagePath(instance, AURORA_STORAGE_SAVES, "");
    instance->config.log_directory = storagePath(instance, AURORA_STORAGE_LOGS, "");
    if (instance->config.save_directory.empty()) {
        logLine(instance, AURORA_LOG_ERROR, "no save directory from the host");
        return AURORA_E_IO;
    }
    logLine(instance, AURORA_LOG_INFO, "script directory: " + instance->config.game_directory);
    return AURORA_OK;
}

AuroraStatus startInstance(void* handle) {
    Instance* instance = static_cast<Instance*>(handle);
    if (instance == nullptr) return AURORA_E_INVALID_ARGUMENT;
    if (instance->running.load()) return AURORA_E_STATE;
    if (!aurora::ons::engineAvailable()) {
        logLine(instance, AURORA_LOG_WARNING, "this build has no ONScripter engine linked");
        return AURORA_E_UNSUPPORTED;
    }

    /* SDL has to come up on the caller's thread (main on iOS); only the script
     * loop moves to the worker. */
    const int initCode = aurora::ons::initEngine(instance->config);
    if (initCode != 0) {
        logLine(instance, AURORA_LOG_ERROR, "engine initialisation failed (" + std::to_string(initCode) + ")");
        return AURORA_E_INIT_FAILED;
    }

    instance->result.store(0);
    instance->running.store(true);
    instance->worker = std::thread([instance]() {
        const int code = aurora::ons::runLoop();
        aurora::ons::shutdownEngine();
        instance->result.store(code);
        instance->running.store(false);
    });
    const std::string video = aurora::ons::describeVideo();
    if (!video.empty()) {
        logLine(instance, AURORA_LOG_INFO, video);
    }
    logLine(instance, AURORA_LOG_INFO, "engine thread started");
    return AURORA_OK;
}

AuroraStatus pauseInstance(void* handle) { return handle == nullptr ? AURORA_E_INVALID_ARGUMENT : AURORA_E_UNSUPPORTED; }
AuroraStatus resumeInstance(void* handle) { return handle == nullptr ? AURORA_E_INVALID_ARGUMENT : AURORA_E_UNSUPPORTED; }

AuroraStatus stopInstance(void* handle) {
    Instance* instance = static_cast<Instance*>(handle);
    if (instance == nullptr) return AURORA_E_INVALID_ARGUMENT;
    aurora::ons::requestStop();
    if (instance->worker.joinable()) instance->worker.join();
    instance->running.store(false);
    logLine(instance, AURORA_LOG_INFO, "engine thread stopped");
    return AURORA_OK;
}

AuroraStatus queryStatus(void* handle, AuroraModuleInstanceStatus* out_status) {
    if (handle == nullptr || out_status == nullptr) return AURORA_E_INVALID_ARGUMENT;
    Instance* instance = static_cast<Instance*>(handle);
    out_status->state = instance->running.load() ? AURORA_STATE_RUNNING : AURORA_STATE_READY;
    out_status->frames_presented = 0;
    out_status->frames_dropped = 0;
    out_status->average_fps = 0.0;
    out_status->name = instance->status_name.c_str();
    return AURORA_OK;
}

void releaseInstance(void* handle) {
    Instance* instance = static_cast<Instance*>(handle);
    if (instance == nullptr) return;
    if (instance->worker.joinable()) {
        aurora::ons::requestStop();
        instance->worker.join();
    }
    delete instance;
}

/* When the engine thread finishes on its own the supervisor needs to see it, so
 * the tick observes the thread rather than producing frames. */
AuroraStatus tickInstance(void* handle, double) {
    Instance* instance = static_cast<Instance*>(handle);
    if (instance == nullptr) return AURORA_E_INVALID_ARGUMENT;
    if (!instance->running.load() && instance->worker.joinable()) {
        const int code = instance->result.load();
        instance->worker.join();
        if (code != 0) {
            logLine(instance, AURORA_LOG_WARNING, "engine exited with code " + std::to_string(code));
        }
    }
    return AURORA_OK;
}

const AuroraModuleVTable kTable = {
    sizeof(AuroraModuleVTable), AURORA_ABI_MAJOR, AURORA_ABI_MINOR,
    kModuleId, "0.1.0",
    &createInstance, &loadGame, &startInstance, &pauseInstance, &resumeInstance,
    &stopInstance, &queryStatus, &releaseInstance,
    &tickInstance,
};

} // namespace

AURORA_MODULE_ENTRY(onscripter) {
    return &kTable;
}
