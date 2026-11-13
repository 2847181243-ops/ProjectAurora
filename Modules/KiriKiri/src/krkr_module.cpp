// SPDX-License-Identifier: GPL-3.0-or-later
#include <cstring>
#include <fstream>
#include <new>
#include <string>

#include "aurora/module_api.h"
#include "krkr_engine.h"

namespace {

const char* kModuleId = "com.aurora.runtime.kirikiri";

struct Instance {
    const AuroraHostServices* host = nullptr;
    aurora::krkr::Config config;
    bool running = false;
    std::string name = "kirikiri";
    int64_t frames = 0;
    bool touch_down = false;
    int32_t touch_x = -1;
    int32_t touch_y = -1;
};

void logLine(Instance* instance, AuroraLogLevel level, const std::string& message) {
    if (instance->host != nullptr && instance->host->write_log != nullptr) {
        instance->host->write_log(instance->host->host_context, kModuleId, level, message.c_str());
    }
}

std::string storagePath(Instance* instance, AuroraStorageKind kind) {
    if (instance->host == nullptr || instance->host->storage_directory == nullptr) return std::string();
    char buffer[1024];
    const AuroraStatus status = instance->host->storage_directory(
        instance->host->host_context, kModuleId, kind, "", buffer, sizeof(buffer));
    return status == AURORA_OK ? std::string(buffer) : std::string();
}

/* The host log goes to the device console, which a simulator job cannot read from
 * the unified log; dropping the text next to the cache keeps it retrievable. */
void writeStartupLog(Instance* instance, const std::string& text) {
    if (instance == nullptr || instance->config.cache_directory.empty()) return;
    std::ofstream out(instance->config.cache_directory + "/krkr-startup.log", std::ios::trunc);
    if (out) out << text;
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
    instance->config.game_root = game->game_path;
    instance->config.writable_directory = storagePath(instance, AURORA_STORAGE_SAVES);
    instance->config.cache_directory = storagePath(instance, AURORA_STORAGE_STATES);
    return AURORA_OK;
}

AuroraStatus startInstance(void* handle) {
    Instance* instance = static_cast<Instance*>(handle);
    if (instance == nullptr) return AURORA_E_INVALID_ARGUMENT;
    if (!aurora::krkr::engineAvailable()) {
        logLine(instance, AURORA_LOG_WARNING, "this build has no KiriKiri engine linked");
        return AURORA_E_UNSUPPORTED;
    }
    std::string error;
    if (!aurora::krkr::create(instance->config, error)) {
        logLine(instance, AURORA_LOG_ERROR, "engine_create failed: " + error);
        return AURORA_E_INIT_FAILED;
    }
    if (!aurora::krkr::openGame(instance->config.game_root, error)) {
        logLine(instance, AURORA_LOG_ERROR, "engine_open_game failed: " + error);
        writeStartupLog(instance, error);
        aurora::krkr::destroy();
        return AURORA_E_IO;
    }
    instance->running = true;
    logLine(instance, AURORA_LOG_INFO, "engine started");
    return AURORA_OK;
}

AuroraStatus pauseInstance(void* handle) { return handle == nullptr ? AURORA_E_INVALID_ARGUMENT : AURORA_E_UNSUPPORTED; }
AuroraStatus resumeInstance(void* handle) { return handle == nullptr ? AURORA_E_INVALID_ARGUMENT : AURORA_E_UNSUPPORTED; }

AuroraStatus stopInstance(void* handle) {
    Instance* instance = static_cast<Instance*>(handle);
    if (instance == nullptr) return AURORA_E_INVALID_ARGUMENT;
    aurora::krkr::destroy();
    instance->running = false;
    return AURORA_OK;
}

AuroraStatus queryStatus(void* handle, AuroraModuleInstanceStatus* out_status) {
    if (handle == nullptr || out_status == nullptr) return AURORA_E_INVALID_ARGUMENT;
    Instance* instance = static_cast<Instance*>(handle);
    out_status->state = instance->running ? AURORA_STATE_RUNNING : AURORA_STATE_READY;
    out_status->frames_presented = instance->frames;
    out_status->frames_dropped = 0;
    out_status->average_fps = 0.0;
    out_status->name = instance->name.c_str();
    return AURORA_OK;
}

void releaseInstance(void* handle) {
    Instance* instance = static_cast<Instance*>(handle);
    if (instance == nullptr) return;
    aurora::krkr::destroy();
    delete instance;
}

/* One tick: advance the engine, then hand the pixels to the host surface. */
AuroraStatus tickInstance(void* handle, double delta_seconds) {
    Instance* instance = static_cast<Instance*>(handle);
    if (instance == nullptr) return AURORA_E_INVALID_ARGUMENT;
    if (!instance->running) return AURORA_OK;

    /* Touch arrives as surface pixels; the engine wants the same coordinate
     * space, so the host input snapshot needs no further mapping. */
    AuroraInputState input{};
    if (instance->host->poll_input != nullptr &&
        instance->host->poll_input(instance->host->host_context, &input) == AURORA_OK) {
        const bool pressed = input.touch_active != 0;
        const bool moved = pressed && (input.touch_x != instance->touch_x || input.touch_y != instance->touch_y);
        if (pressed && !instance->touch_down) {
            aurora::krkr::sendPointer(0, static_cast<float>(input.touch_x), static_cast<float>(input.touch_y));
            instance->touch_down = true;
            instance->touch_x = input.touch_x;
            instance->touch_y = input.touch_y;
        } else if (pressed && moved) {
            aurora::krkr::sendPointer(1, static_cast<float>(input.touch_x), static_cast<float>(input.touch_y));
            instance->touch_x = input.touch_x;
            instance->touch_y = input.touch_y;
        } else if (!pressed && instance->touch_down) {
            aurora::krkr::sendPointer(2, static_cast<float>(instance->touch_x), static_cast<float>(instance->touch_y));
            instance->touch_down = false;
        }
    }

    uint32_t delta_ms = static_cast<uint32_t>(delta_seconds * 1000.0);
    if (delta_ms == 0) delta_ms = 1;

    aurora::krkr::Frame frame;
    std::string error;
    if (!aurora::krkr::tick(delta_ms, frame, error)) {
        if (!error.empty()) logLine(instance, AURORA_LOG_WARNING, "tick failed: " + error);
        return AURORA_OK;
    }
    if (frame.pixels == nullptr || frame.width == 0 || frame.height == 0) return AURORA_OK;

    AuroraFrame host_frame{};
    if (instance->host->acquire_frame(instance->host->host_context, frame.width, frame.height, &host_frame) != AURORA_OK) {
        return AURORA_OK; /* headless host: nothing to draw into */
    }

    const uint32_t rows = frame.height < host_frame.height ? frame.height : host_frame.height;
    const uint32_t copy_bytes = frame.width * 4 < host_frame.stride_bytes ? frame.width * 4 : host_frame.stride_bytes;
    for (uint32_t row = 0; row < rows; ++row) {
        std::memcpy(static_cast<uint8_t*>(host_frame.pixels) + static_cast<std::size_t>(row) * host_frame.stride_bytes,
                    frame.pixels + static_cast<std::size_t>(row) * frame.stride_bytes, copy_bytes);
    }
    instance->host->present_frame(instance->host->host_context, &host_frame);
    ++instance->frames;
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

AURORA_MODULE_ENTRY(kirikiri) {
    return &kTable;
}
