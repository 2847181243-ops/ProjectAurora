// SPDX-License-Identifier: GPL-3.0-or-later
/* The real side of the KiriKiri seam.
 *
 * KiriKiri is tick driven, so this is a thin translation layer: create the
 * engine, open the game, advance it by a delta and hand the RGBA frame back.
 */
#if defined(AURORA_WITH_KRKR)

#include <cstring>
#include <vector>

#include <engine_api.h>

#include "krkr_engine.h"

namespace aurora {
namespace krkr {

namespace {

engine_handle_t g_engine = nullptr;
std::vector<uint8_t> g_pixels;

/* The engine keeps its startup output in a ring buffer; draining it is the only way
 * to learn why open_game failed. */
std::string drainStartupLogs() {
    if (g_engine == nullptr) return std::string();
    std::string all;
    std::vector<char> chunk(2048);
    for (int i = 0; i < 16; ++i) {
        uint32_t written = 0;
        chunk[0] = '\0';
        if (engine_drain_startup_logs(g_engine, chunk.data(), static_cast<uint32_t>(chunk.size()), &written) != ENGINE_RESULT_OK) break;
        if (written == 0) break;
        all.append(chunk.data(), written);
    }
    return all;
}

std::string lastError() {
    const char* message = g_engine != nullptr ? engine_get_last_error(g_engine) : nullptr;
    return message != nullptr ? std::string(message) : std::string("engine error");
}

} // namespace

bool engineAvailable() { return true; }

bool create(const Config& config, std::string& error) {
    uint32_t api_version = 0;
    if (engine_get_runtime_api_version(&api_version) != ENGINE_RESULT_OK) {
        error = "engine_get_runtime_api_version failed";
        return false;
    }

    engine_create_desc_t desc;
    std::memset(&desc, 0, sizeof(desc));
    desc.struct_size = sizeof(desc);
    desc.api_version = api_version;
    desc.writable_path_utf8 = config.writable_directory.c_str();
    desc.cache_path_utf8 = config.cache_directory.c_str();

    engine_handle_t handle = nullptr;
    const engine_result_t result = engine_create(&desc, &handle);
    if (result != ENGINE_RESULT_OK || handle == nullptr) {
        error = "engine_create returned " + std::to_string(static_cast<int>(result));
        return false;
    }
    g_engine = handle;
    return true;
}

bool openGame(const std::string& game_root, std::string& error) {
    if (g_engine == nullptr) { error = "no engine"; return false; }
    const engine_result_t result = engine_open_game(g_engine, game_root.c_str(), nullptr);
    if (result != ENGINE_RESULT_OK) {
        error = lastError();
        const std::string logs = drainStartupLogs();
        if (!logs.empty()) error += "\n" + logs;
        return false;
    }
    return true;
}

bool tick(uint32_t delta_ms, Frame& out, std::string& error) {
    if (g_engine == nullptr) { error = "no engine"; return false; }

    if (engine_tick(g_engine, delta_ms) != ENGINE_RESULT_OK) {
        error = lastError();
        return false;
    }

    engine_frame_desc_t frame_desc;
    std::memset(&frame_desc, 0, sizeof(frame_desc));
    frame_desc.struct_size = sizeof(frame_desc);
    if (engine_get_frame_desc(g_engine, &frame_desc) != ENGINE_RESULT_OK) {
        error = lastError();
        return false;
    }
    if (frame_desc.width == 0 || frame_desc.height == 0 || frame_desc.stride_bytes == 0) {
        return false; /* nothing rendered yet */
    }

    const std::size_t needed = static_cast<std::size_t>(frame_desc.stride_bytes) * frame_desc.height;
    if (g_pixels.size() < needed) g_pixels.resize(needed);

    if (engine_read_frame_rgba(g_engine, g_pixels.data(), g_pixels.size()) != ENGINE_RESULT_OK) {
        error = lastError();
        return false;
    }

    out.pixels = g_pixels.data();
    out.width = frame_desc.width;
    out.height = frame_desc.height;
    out.stride_bytes = frame_desc.stride_bytes;
    return true;
}

void sendPointer(int kind, float x, float y) {
    if (g_engine == nullptr) return;
    engine_input_event_t event;
    std::memset(&event, 0, sizeof(event));
    event.struct_size = sizeof(event);
    switch (kind) {
        case 0: event.type = ENGINE_INPUT_EVENT_POINTER_DOWN; break;
        case 1: event.type = ENGINE_INPUT_EVENT_POINTER_MOVE; break;
        case 2: event.type = ENGINE_INPUT_EVENT_POINTER_UP; break;
        default: return;
    }
    event.x = x;
    event.y = y;
    event.pointer_id = 0;
    engine_send_input(g_engine, &event);
}

void setSurfaceSize(uint32_t width, uint32_t height) {
    if (g_engine != nullptr) engine_set_surface_size(g_engine, width, height);
}

bool pause() {
    if (g_engine == nullptr) return false;
    return engine_pause(g_engine) == ENGINE_RESULT_OK;
}

bool resume() {
    if (g_engine == nullptr) return false;
    return engine_resume(g_engine) == ENGINE_RESULT_OK;
}

void destroy() {
    if (g_engine != nullptr) {
        engine_destroy(g_engine);
        g_engine = nullptr;
    }
    g_pixels.clear();
}

} // namespace krkr
} // namespace aurora

#endif /* AURORA_WITH_KRKR */
