/* The smallest possible module: proves the ABI can be implemented in one screen
 * of code and that "a module that does nothing" still behaves like a citizen. */
#include <cstring>

#include <new>

#include "aurora/module_api.h"

namespace {

const char* kModuleId = "com.aurora.runtime.empty";

struct Instance {
    const AuroraHostServices* host = nullptr;
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
    if (handle == nullptr || game == nullptr) return AURORA_E_INVALID_ARGUMENT;
    return AURORA_OK;
}

AuroraStatus startInstance(void* handle) {
    Instance* instance = static_cast<Instance*>(handle);
    if (instance == nullptr) return AURORA_E_INVALID_ARGUMENT;
    instance->started = true;
    logLine(instance, AURORA_LOG_INFO, "empty module started; it exists and stays quiet");
    return AURORA_OK;
}

AuroraStatus pauseInstance(void* handle) { return handle == nullptr ? AURORA_E_INVALID_ARGUMENT : AURORA_OK; }
AuroraStatus resumeInstance(void* handle) { return handle == nullptr ? AURORA_E_INVALID_ARGUMENT : AURORA_OK; }

AuroraStatus stopInstance(void* handle) {
    Instance* instance = static_cast<Instance*>(handle);
    if (instance == nullptr) return AURORA_E_INVALID_ARGUMENT;
    instance->started = false;
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
    nullptr /* no manual tick */,
};

} // namespace

AURORA_MODULE_ENTRY(empty) {
    return &kTable;
}
