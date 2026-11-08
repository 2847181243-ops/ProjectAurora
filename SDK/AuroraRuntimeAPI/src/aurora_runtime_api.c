#include "aurora/module_api.h"

const char* aurora_status_string(AuroraStatus status) {
    switch (status) {
        case AURORA_OK: return "ok";
        case AURORA_E_INVALID_ARGUMENT: return "invalid_argument";
        case AURORA_E_UNSUPPORTED: return "unsupported";
        case AURORA_E_UNSUPPORTED_GAME: return "unsupported_game";
        case AURORA_E_MISSING_FIRMWARE: return "missing_firmware";
        case AURORA_E_IO: return "io_error";
        case AURORA_E_OUT_OF_MEMORY: return "out_of_memory";
        case AURORA_E_INIT_FAILED: return "init_failed";
        case AURORA_E_STATE: return "illegal_state";
        case AURORA_E_TIMEOUT: return "timeout";
        case AURORA_E_INTERNAL: return "internal_error";
        case AURORA_E_CANCELLED: return "cancelled";
        case AURORA_E_ABI_MISMATCH: return "abi_mismatch";
        default: return "unknown";
    }
}

const char* aurora_module_state_string(AuroraModuleState state) {
    switch (state) {
        case AURORA_STATE_DISCOVERED: return "DISCOVERED";
        case AURORA_STATE_PLANNED: return "PLANNED";
        case AURORA_STATE_BUILDING: return "BUILDING";
        case AURORA_STATE_BUILT: return "BUILT";
        case AURORA_STATE_READY: return "READY";
        case AURORA_STATE_RUNNING: return "RUNNING";
        case AURORA_STATE_DISABLED: return "DISABLED";
        case AURORA_STATE_FAILED: return "FAILED";
        case AURORA_STATE_DEPRECATED: return "DEPRECATED";
        default: return "UNKNOWN";
    }
}

const char* aurora_runtime_kind_string(AuroraRuntimeKind kind) {
    switch (kind) {
        case AURORA_KIND_GALGAME: return "galgame_engine";
        case AURORA_KIND_CONSOLE_EMULATOR: return "console_emulator";
        case AURORA_KIND_OTHER: return "other";
        default: return "unknown";
    }
}

const char* aurora_storage_kind_string(AuroraStorageKind kind) {
    switch (kind) {
        case AURORA_STORAGE_CONFIG: return "Config";
        case AURORA_STORAGE_SAVES: return "Saves";
        case AURORA_STORAGE_STATES: return "States";
        case AURORA_STORAGE_LOGS: return "Logs";
        default: return "Unknown";
    }
}

AuroraStatus aurora_vtable_validate(const AuroraModuleVTable* vtable) {
    if (vtable == 0) return AURORA_E_INVALID_ARGUMENT;
    if (vtable->abi_major != AURORA_ABI_MAJOR) return AURORA_E_ABI_MISMATCH;
    if (vtable->struct_size < sizeof(AuroraModuleVTable)) return AURORA_E_ABI_MISMATCH;
    if (vtable->module_id == 0 || vtable->module_id[0] == '\0') return AURORA_E_INVALID_ARGUMENT;
    if (vtable->create_instance == 0) return AURORA_E_INVALID_ARGUMENT;
    if (vtable->load_game == 0) return AURORA_E_INVALID_ARGUMENT;
    if (vtable->start == 0) return AURORA_E_INVALID_ARGUMENT;
    if (vtable->stop == 0) return AURORA_E_INVALID_ARGUMENT;
    if (vtable->release_instance == 0) return AURORA_E_INVALID_ARGUMENT;
    return AURORA_OK;
}
