/* A simulated runtime module.
 *
 * It exists to prove the contract, not to emulate anything: it renders animated
 * frames, reads its own config directory, writes a state file to its own bucket
 * and reports honest status. If the host has no display it keeps running and says
 * so once, instead of failing - that is the documented degradation rule.
 */
#include <cstdio>
#include <cstring>
#include <string>
#include <fstream>

#include <new>

#include "aurora/module_api.h"

namespace {

char* duplicate(const char* text) {
    if (text == nullptr) return nullptr;
    const std::size_t size = std::strlen(text) + 1;
    char* copy = new (std::nothrow) char[size];
    if (copy != nullptr) std::memcpy(copy, text, size);
    return copy;
}

bool stringEquals(const char* a, const char* b) {
    if (a == nullptr || b == nullptr) return false;
    return std::strcmp(a, b) == 0;
}

bool formatSupported(const char* format) {
    return stringEquals(format, "sim") || stringEquals(format, "demo");
}

struct Instance {
    const AuroraHostServices* host = nullptr;
    bool loaded = false;
    bool running = false;
    bool paused = false;
    bool surface_available = false;
    bool warned_no_surface = false;
    int64_t frame_index = 0;
    int64_t presented = 0;
    double started_at = 0.0;
    double average_fps = 0.0;
    uint32_t width = 320;
    uint32_t height = 240;
    int32_t bar = 0;
    std::string config_directory;
    std::string save_directory;
    std::string state_directory;
    std::string last_name = "simulated";
};

const char* kModuleId = "com.aurora.runtime.simulated";

void log(Instance* instance, AuroraLogLevel level, const std::string& message) {
    if (instance->host != nullptr && instance->host->write_log != nullptr) {
        instance->host->write_log(instance->host->host_context, kModuleId, level, message.c_str());
    }
}

std::string storagePath(Instance* instance, AuroraStorageKind kind, const char* relative) {
    if (instance->host == nullptr || instance->host->storage_directory == nullptr) return std::string();
    char buffer[1024];
    const AuroraStatus status = instance->host->storage_directory(
        instance->host->host_context, kModuleId, kind, relative, buffer, sizeof(buffer));
    if (status != AURORA_OK) return std::string();
    return std::string(buffer);
}

void renderFrame(Instance* instance, AuroraFrame* frame) {
    unsigned char* pixels = static_cast<unsigned char*>(frame->pixels);
    if (pixels == nullptr) return;
    for (uint32_t y = 0; y < frame->height; ++y) {
        unsigned char* row = pixels + static_cast<std::size_t>(y) * frame->stride_bytes;
        for (uint32_t x = 0; x < frame->width; ++x) {
            const unsigned char gradient = static_cast<unsigned char>((x * 255u) / (frame->width > 1 ? frame->width - 1 : 1));
            const unsigned char vertical = static_cast<unsigned char>((y * 120u) / (frame->height > 1 ? frame->height - 1 : 1));
            const bool in_bar = (x >= static_cast<uint32_t>(instance->bar) &&
                                 x < static_cast<uint32_t>(instance->bar) + 24u);
            row[x * 4 + 0] = in_bar ? 255 : gradient;
            row[x * 4 + 1] = in_bar ? 210 : vertical;
            row[x * 4 + 2] = in_bar ? 40 : static_cast<unsigned char>(255 - gradient);
            row[x * 4 + 3] = 255;
        }
    }
}

void advance(Instance* instance, double delta_seconds) {
    instance->frame_index += 1;
    instance->bar = static_cast<int32_t>((instance->frame_index * 3) % (static_cast<int64_t>(instance->width) + 24));

    AuroraInputState input{};
    if (instance->host != nullptr && instance->host->poll_input != nullptr &&
        instance->host->poll_input(instance->host->host_context, &input) == AURORA_OK) {
        if (input.touch_active != 0 && input.touch_x >= 0 && input.touch_x < static_cast<int32_t>(instance->width)) {
            instance->bar = input.touch_x;
        }
    }

    AuroraFrame frame{};
    if (instance->host != nullptr && instance->host->acquire_frame != nullptr &&
        instance->host->acquire_frame(instance->host->host_context, instance->width, instance->height, &frame) == AURORA_OK) {
        instance->surface_available = true;
        renderFrame(instance, &frame);
        if (instance->host->present_frame != nullptr) {
            instance->host->present_frame(instance->host->host_context, &frame);
        }
        instance->presented += 1;
    } else if (!instance->warned_no_surface) {
        instance->warned_no_surface = true;
        log(instance, AURORA_LOG_INFO, "no display surface on this host; running headless");
    }

    if (instance->host != nullptr && instance->host->monotonic_seconds != nullptr && delta_seconds > 0.0) {
        const double now = instance->host->monotonic_seconds(instance->host->host_context);
        const double elapsed = now - instance->started_at;
        if (elapsed > 0.0) instance->average_fps = static_cast<double>(instance->frame_index) / elapsed;
    }
}

/* ---- ABI ------------------------------------------------------------------ */

AuroraStatus createInstance(const AuroraHostServices* host, void** out_instance) {
    if (host == nullptr || out_instance == nullptr) return AURORA_E_INVALID_ARGUMENT;
    *out_instance = nullptr;
    Instance* instance = new (std::nothrow) Instance();
    if (instance == nullptr) return AURORA_E_OUT_OF_MEMORY;
    instance->host = host;

    instance->config_directory = storagePath(instance, AURORA_STORAGE_CONFIG, "settings.json");
    instance->save_directory = storagePath(instance, AURORA_STORAGE_SAVES, "save.slot1");
    instance->state_directory = storagePath(instance, AURORA_STORAGE_STATES, "state.slot1");

    if (instance->config_directory.empty()) {
        log(instance, AURORA_LOG_ERROR, "config directory unavailable");
    } else {
        std::ifstream config(instance->config_directory);
        std::string line;
        if (config.good() && std::getline(config, line)) {
            log(instance, AURORA_LOG_INFO, "loaded own config: " + line.substr(0, 120));
        } else {
            std::ofstream created(instance->config_directory);
            created << "{\"scale\":\"fit\",\"audio\":{\"volume\":0.8}}\n";
            log(instance, AURORA_LOG_INFO, "wrote default config");
        }
    }
    log(instance, AURORA_LOG_INFO, "instance created");
    *out_instance = instance;
    return AURORA_OK;
}

AuroraStatus loadGame(void* handle, const AuroraGameInfo* game) {
    if (handle == nullptr || game == nullptr || game->game_path == nullptr) return AURORA_E_INVALID_ARGUMENT;
    Instance* instance = static_cast<Instance*>(handle);
    if (!formatSupported(game->format)) {
        log(instance, AURORA_LOG_WARNING, std::string("unsupported format: ") + (game->format ? game->format : ""));
        return AURORA_E_UNSUPPORTED_GAME;
    }
    instance->loaded = true;
    instance->last_name = game->display_name != nullptr ? game->display_name : "simulated";
    log(instance, AURORA_LOG_INFO, std::string("loaded ") + game->game_path);
    return AURORA_OK;
}

AuroraStatus startInstance(void* handle) {
    Instance* instance = static_cast<Instance*>(handle);
    if (instance == nullptr) return AURORA_E_INVALID_ARGUMENT;
    if (!instance->loaded) return AURORA_E_STATE;
    instance->running = true;
    instance->paused = false;
    instance->started_at = instance->host != nullptr && instance->host->monotonic_seconds != nullptr
                               ? instance->host->monotonic_seconds(instance->host->host_context)
                               : 0.0;
    log(instance, AURORA_LOG_INFO, "started");
    return AURORA_OK;
}

AuroraStatus pauseInstance(void* handle) {
    Instance* instance = static_cast<Instance*>(handle);
    if (instance == nullptr) return AURORA_E_INVALID_ARGUMENT;
    if (!instance->running) return AURORA_E_STATE;
    instance->paused = true;
    log(instance, AURORA_LOG_INFO, "paused");
    return AURORA_OK;
}

AuroraStatus resumeInstance(void* handle) {
    Instance* instance = static_cast<Instance*>(handle);
    if (instance == nullptr) return AURORA_E_INVALID_ARGUMENT;
    if (!instance->running) return AURORA_E_STATE;
    instance->paused = false;
    log(instance, AURORA_LOG_INFO, "resumed");
    return AURORA_OK;
}

AuroraStatus stopInstance(void* handle) {
    Instance* instance = static_cast<Instance*>(handle);
    if (instance == nullptr) return AURORA_E_INVALID_ARGUMENT;
    if (!instance->running) return AURORA_E_STATE;
    instance->running = false;
    instance->paused = false;

    if (!instance->state_directory.empty()) {
        std::ofstream state(instance->state_directory);
        state << "{\"frames\":" << instance->frame_index << ",\"game\":\"" << instance->last_name << "\"}\n";
    }
    if (!instance->save_directory.empty()) {
        std::ofstream save(instance->save_directory);
        save << "aurora-sim-save-v1 frames=" << instance->frame_index << "\n";
    }
    log(instance, AURORA_LOG_INFO, "stopped");
    return AURORA_OK;
}

AuroraStatus queryStatus(void* handle, AuroraModuleInstanceStatus* out_status) {
    Instance* instance = static_cast<Instance*>(handle);
    if (instance == nullptr || out_status == nullptr) return AURORA_E_INVALID_ARGUMENT;
    out_status->state = instance->running ? AURORA_STATE_RUNNING : instance->loaded ? AURORA_STATE_READY
                                                                                    : AURORA_STATE_BUILT;
    out_status->frames_presented = instance->presented;
    out_status->frames_dropped = 0;
    out_status->average_fps = instance->average_fps;
    out_status->name = instance->last_name.c_str();
    return AURORA_OK;
}

void releaseInstance(void* handle) {
    Instance* instance = static_cast<Instance*>(handle);
    if (instance == nullptr) return;
    if (instance->running) {
        stopInstance(handle);
    }
    delete instance;
}

AuroraStatus tickInstance(void* handle, double delta_seconds) {
    Instance* instance = static_cast<Instance*>(handle);
    if (instance == nullptr) return AURORA_E_INVALID_ARGUMENT;
    if (!instance->running) return AURORA_E_STATE;
    if (instance->paused) return AURORA_OK;
    advance(instance, delta_seconds);
    return AURORA_OK;
}

const AuroraModuleVTable kTable = {
    sizeof(AuroraModuleVTable),
    AURORA_ABI_MAJOR,
    AURORA_ABI_MINOR,
    kModuleId,
    "1.0.0",
    &createInstance,
    &loadGame,
    &startInstance,
    &pauseInstance,
    &resumeInstance,
    &stopInstance,
    &queryStatus,
    &releaseInstance,
    &tickInstance,
};

} // namespace

AURORA_MODULE_ENTRY(simulated) {
    return &kTable;
}
