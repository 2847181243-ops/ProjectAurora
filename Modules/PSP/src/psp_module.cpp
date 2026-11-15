// SPDX-License-Identifier: GPL-3.0-or-later
#include <cstdio>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <new>
#include <string>
#include <vector>

#include "aurora/module_api.h"
#include "psp_engine.h"

namespace {

const char* kModuleId = "com.aurora.runtime.ppsspp";

struct Instance {
    const AuroraHostServices* host = nullptr;
    aurora::psp::Config config;
    std::vector<uint8_t> pending;   /* last frame handed over by the core */
    int frame_width = 0;
    int frame_height = 0;
    int64_t frames = 0;
    std::string status_name = "ppsspp";
};

std::string storagePath(Instance* instance, AuroraStorageKind kind);

/* Device console is unreadable in practice; every core message also goes to a
 * file so the failure point can be inspected afterwards. */
void logToFile(Instance* instance, const std::string& message) {
    if (instance == nullptr) return;
    const std::string dir = storagePath(instance, AURORA_STORAGE_SAVES);
    if (dir.empty()) return;
    std::ofstream out(dir + "/ppsspp.log", std::ios::app);
    if (out) out << message << "\n";
}

void logLine(Instance* instance, AuroraLogLevel level, const std::string& message) {
    logToFile(instance, message);
    std::fprintf(stderr, "[ppsspp] %s\n", message.c_str());
    if (instance->host != nullptr && instance->host->write_log != nullptr) {
        instance->host->write_log(instance->host->host_context, kModuleId, level, message.c_str());
    }
}

std::string storagePath(Instance* instance, AuroraStorageKind kind) {
    if (instance->host == nullptr || instance->host->storage_directory == nullptr) return {};
    char buffer[1024];
    const AuroraStatus status = instance->host->storage_directory(
        instance->host->host_context, kModuleId, kind, "", buffer, sizeof(buffer));
    return status == AURORA_OK ? std::string(buffer) : std::string();
}

void onFrame(void* context, const void* pixels, int width, int height, int stride) {
    auto* instance = static_cast<Instance*>(context);
    if (instance == nullptr || pixels == nullptr || width <= 0 || height <= 0) return;
    instance->frame_width = width;
    instance->frame_height = height;
    instance->pending.resize(static_cast<std::size_t>(width) * static_cast<std::size_t>(height) * 4u);
    for (int row = 0; row < height; ++row) {
        std::memcpy(instance->pending.data() + static_cast<std::size_t>(row) * width * 4u,
                    static_cast<const uint8_t*>(pixels) + static_cast<std::size_t>(row) * stride,
                    static_cast<std::size_t>(width) * 4u);
    }
}

void onLog(void* context, int level, const char* message) {
    auto* instance = static_cast<Instance*>(context);
    if (instance == nullptr || message == nullptr) return;
    logLine(instance, level <= 0 ? AURORA_LOG_ERROR : AURORA_LOG_INFO, message);
}

/* The core asks "is this button down right now" every frame; answer from the
 * host snapshot the module stored on the last poll. */
int onButton(void* context, unsigned id) {
    auto* instance = static_cast<Instance*>(context);
    if (instance == nullptr || instance->host == nullptr || instance->host->poll_input == nullptr) return 0;
    AuroraInputState state{};
    if (instance->host->poll_input(instance->host->host_context, &state) != AURORA_OK) return 0;
    const bool held = state.touch_active != 0;
    switch (id) {
        case 0: return held;      /* RETRO_DEVICE_ID_JOYPAD_B: accept */
        case 8: return held;      /* SELECT */
        case 9: return 0;         /* START */
        default: return 0;
    }
}

AuroraStatus createInstance(const AuroraHostServices* host, void** out_instance) {
    if (host == nullptr || out_instance == nullptr) return AURORA_E_INVALID_ARGUMENT;
    auto* instance = new (std::nothrow) Instance();
    if (instance == nullptr) return AURORA_E_OUT_OF_MEMORY;
    instance->host = host;
    *out_instance = instance;
    return AURORA_OK;
}

AuroraStatus loadGame(void* handle, const AuroraGameInfo* game) {
    if (handle == nullptr || game == nullptr || game->game_path == nullptr) return AURORA_E_INVALID_ARGUMENT;
    auto* instance = static_cast<Instance*>(handle);
    instance->config.game_path = game->game_path;
    instance->config.save_directory = storagePath(instance, AURORA_STORAGE_SAVES);
    instance->config.system_directory = storagePath(instance, AURORA_STORAGE_SAVES);
    return AURORA_OK;
}

AuroraStatus startInstance(void* handle) {
    auto* instance = static_cast<Instance*>(handle);
    if (instance == nullptr) return AURORA_E_INVALID_ARGUMENT;
    if (!aurora::psp::engineAvailable()) {
        logLine(instance, AURORA_LOG_ERROR, "this build has no PPSSPP core linked");
        return AURORA_E_UNSUPPORTED;
    }
    aurora::psp::Host host;
    host.context = instance;
    host.frame = &onFrame;
    host.log = &onLog;
    host.button = &onButton;
    const int code = aurora::psp::startEngine(instance->config, host);
    if (code != 0) {
        logLine(instance, AURORA_LOG_ERROR, "core start failed (" + std::to_string(code) + ")");
        return AURORA_E_INIT_FAILED;
    }
    logLine(instance, AURORA_LOG_INFO, std::string("core started: ") + aurora::psp::coreName());
    return AURORA_OK;
}

AuroraStatus pauseInstance(void* handle) { return handle == nullptr ? AURORA_E_INVALID_ARGUMENT : AURORA_E_UNSUPPORTED; }
AuroraStatus resumeInstance(void* handle) { return handle == nullptr ? AURORA_E_INVALID_ARGUMENT : AURORA_E_UNSUPPORTED; }

AuroraStatus stopInstance(void* handle) {
    auto* instance = static_cast<Instance*>(handle);
    if (instance == nullptr) return AURORA_E_INVALID_ARGUMENT;
    aurora::psp::stopEngine();
    instance->pending.clear();
    instance->frame_width = 0;
    instance->frame_height = 0;
    return AURORA_OK;
}

AuroraStatus queryStatus(void* handle, AuroraModuleInstanceStatus* out_status) {
    if (handle == nullptr || out_status == nullptr) return AURORA_E_INVALID_ARGUMENT;
    auto* instance = static_cast<Instance*>(handle);
    out_status->state = instance->frame_width > 0 ? AURORA_STATE_RUNNING : AURORA_STATE_READY;
    out_status->frames_presented = instance->frames;
    out_status->frames_dropped = 0;
    out_status->average_fps = 0.0;
    out_status->name = instance->status_name.c_str();
    return AURORA_OK;
}

void releaseInstance(void* handle) {
    auto* instance = static_cast<Instance*>(handle);
    if (instance == nullptr) return;
    aurora::psp::stopEngine();
    delete instance;
}

AuroraStatus tickInstance(void* handle, double) {
    auto* instance = static_cast<Instance*>(handle);
    if (instance == nullptr) return AURORA_E_INVALID_ARGUMENT;
    if (!aurora::psp::engineAvailable()) return AURORA_OK;

    aurora::psp::runFrame();
    if (instance->pending.empty() || instance->host == nullptr || instance->host->acquire_frame == nullptr) {
        return AURORA_OK;
    }
    AuroraFrame frame{};
    if (instance->host->acquire_frame(instance->host->host_context,
                                      static_cast<uint32_t>(instance->frame_width),
                                      static_cast<uint32_t>(instance->frame_height),
                                      &frame) != AURORA_OK) {
        return AURORA_OK;
    }
    const uint32_t rows = static_cast<uint32_t>(instance->frame_height);
    const std::size_t row_bytes = static_cast<std::size_t>(instance->frame_width) * 4u;
    const std::size_t copy_bytes = row_bytes < frame.stride_bytes ? row_bytes : frame.stride_bytes;
    for (uint32_t row = 0; row < rows; ++row) {
        std::memcpy(static_cast<uint8_t*>(frame.pixels) + static_cast<std::size_t>(row) * frame.stride_bytes,
                    instance->pending.data() + static_cast<std::size_t>(row) * row_bytes,
                    copy_bytes);
    }
    instance->host->present_frame(instance->host->host_context, &frame);
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

AURORA_MODULE_ENTRY(ppsspp) {
    return &kTable;
}
