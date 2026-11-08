/* A module that exists only inside the test binary.
 *
 * It gives the registry tests a module with a dependency and a switchable state,
 * without polluting the shipped manifest set.
 */
#include <cstring>

#include <new>

#include "aurora/module_api.h"

namespace {

const char* kModuleId = "com.aurora.test.dependent";

struct Instance {
    const AuroraHostServices* host = nullptr;
    bool started = false;
};

AuroraStatus createInstance(const AuroraHostServices* host, void** out_instance) {
    if (host == nullptr || out_instance == nullptr) return AURORA_E_INVALID_ARGUMENT;
    Instance* instance = new (std::nothrow) Instance();
    if (instance == nullptr) return AURORA_E_OUT_OF_MEMORY;
    instance->host = host;
    *out_instance = instance;
    return AURORA_OK;
}

AuroraStatus loadGame(void* handle, const AuroraGameInfo*) {
    return handle == nullptr ? AURORA_E_INVALID_ARGUMENT : AURORA_OK;
}

AuroraStatus startInstance(void* handle) {
    Instance* instance = static_cast<Instance*>(handle);
    if (instance == nullptr) return AURORA_E_INVALID_ARGUMENT;
    instance->started = true;
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

AuroraStatus queryStatus(void* handle, AuroraModuleInstanceStatus* out) {
    if (handle == nullptr || out == nullptr) return AURORA_E_INVALID_ARGUMENT;
    Instance* instance = static_cast<Instance*>(handle);
    out->state = instance->started ? AURORA_STATE_RUNNING : AURORA_STATE_BUILT;
    out->frames_presented = 0;
    out->frames_dropped = 0;
    out->average_fps = 0.0;
    out->name = kModuleId;
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

AURORA_MODULE_ENTRY(testdependent) {
    return &kTable;
}
