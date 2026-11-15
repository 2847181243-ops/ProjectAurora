// SPDX-License-Identifier: GPL-3.0-or-later
#include "psp_engine.h"

/* The real seam. It only compiles when the PPSSPP libretro headers are on the
 * include path; without them the stub in psp_engine_stub.cpp is used instead. */
#if __has_include("libretro.h")
#include "libretro.h"

#include <cstring>
#include <string>
#include <vector>

namespace aurora::psp {
namespace {

Host g_host{};
Config g_config{};
bool g_loaded = false;
unsigned g_pixel_format = RETRO_PIXEL_FORMAT_0RGB1555;
std::vector<uint8_t> g_frame;
int g_width = 0;
int g_height = 0;

const char* const kVariables[] = {
    "ppsspp_vertex_cache = 1",
    "ppsspp_fast_memory = 1",
    "ppsspp_cpu_clock = 0",
    "ppsspp_software_rendering = 1",
    nullptr,
};

bool environment(unsigned cmd, void* data) {
    switch (cmd) {
        case RETRO_ENVIRONMENT_SET_PIXEL_FORMAT:
            if (data != nullptr) g_pixel_format = *static_cast<const unsigned*>(data);
            return g_pixel_format == RETRO_PIXEL_FORMAT_0RGB1555 ||
                   g_pixel_format == RETRO_PIXEL_FORMAT_RGB565 ||
                   g_pixel_format == RETRO_PIXEL_FORMAT_XRGB8888;
        case RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY:
        case RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY:
            if (data != nullptr) *static_cast<const char**>(data) = g_config.save_directory.c_str();
            return !g_config.save_directory.empty();
        case RETRO_ENVIRONMENT_GET_VARIABLE: {
            auto* variable = static_cast<retro_variable*>(data);
            if (variable == nullptr || variable->key == nullptr) return false;
            for (const char* line : kVariables) {
                if (line == nullptr) break;
                const char* eq = std::strstr(line, " = ");
                if (eq == nullptr) continue;
                const std::size_t keyLength = static_cast<std::size_t>(eq - line);
                if (std::strncmp(line, variable->key, keyLength) == 0 && variable->key[keyLength] == '\0') {
                    variable->value = eq + 3;
                    return true;
                }
            }
            return false;
        }
        case RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE:
            if (data != nullptr) *static_cast<bool*>(data) = false;
            return true;
        case RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME:
            return true;
        case RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS:
        case RETRO_ENVIRONMENT_SET_CONTROLLER_INFO:
        case RETRO_ENVIRONMENT_SET_PERFORMANCE_LEVEL:
        case RETRO_ENVIRONMENT_SET_MEMORY_MAPS:
        case RETRO_ENVIRONMENT_SET_GEOMETRY:
            return true;
        default:
            return false;
    }
}

void videoRefresh(const void* data, unsigned width, unsigned height, std::size_t pitch) {
    if (data == nullptr || width == 0 || height == 0) return;
    g_width = static_cast<int>(width);
    g_height = static_cast<int>(height);
    g_frame.resize(static_cast<std::size_t>(width) * height * 4u);
    const auto* source = static_cast<const uint8_t*>(data);
    for (unsigned row = 0; row < height; ++row) {
        const auto* line = source + static_cast<std::size_t>(row) * pitch;
        uint8_t* out = g_frame.data() + static_cast<std::size_t>(row) * width * 4u;
        for (unsigned x = 0; x < width; ++x) {
            if (g_pixel_format == RETRO_PIXEL_FORMAT_XRGB8888) {
                const uint32_t pixel = reinterpret_cast<const uint32_t*>(line)[x];
                out[x * 4 + 0] = static_cast<uint8_t>((pixel >> 16) & 0xff);
                out[x * 4 + 1] = static_cast<uint8_t>((pixel >> 8) & 0xff);
                out[x * 4 + 2] = static_cast<uint8_t>(pixel & 0xff);
                out[x * 4 + 3] = 0xff;
            } else {
                const uint16_t pixel = reinterpret_cast<const uint16_t*>(line)[x];
                out[x * 4 + 0] = static_cast<uint8_t>(((pixel >> 10) & 0x1f) << 3);
                out[x * 4 + 1] = static_cast<uint8_t>(((pixel >> 5) & 0x1f) << 3);
                out[x * 4 + 2] = static_cast<uint8_t>((pixel & 0x1f) << 3);
                out[x * 4 + 3] = 0xff;
            }
        }
    }
    if (g_host.frame != nullptr) g_host.frame(g_host.context, g_frame.data(), g_width, g_height, g_width * 4);
}

void audioSample(int16_t, int16_t) {}
std::size_t audioBatch(const int16_t*, std::size_t frames) { return frames; }
void inputPoll() {}
int16_t inputState(unsigned port, unsigned device, unsigned index, unsigned id) {
    if (port != 0 || g_host.button == nullptr) return 0;
    return static_cast<int16_t>(g_host.button(g_host.context, id) ? 1 : 0);
}

} // namespace

const char* coreName() { return "PPSSPP libretro"; }
bool engineAvailable() { return true; }

int startEngine(const Config& config, const Host& host) {
    g_config = config;
    g_host = host;

    retro_set_environment(&environment);
    retro_set_video_refresh(&videoRefresh);
    retro_set_audio_sample(&audioSample);
    retro_set_audio_sample_batch(&audioBatch);
    retro_set_input_poll(&inputPoll);
    retro_set_input_state(&inputState);
    retro_init();

    retro_system_info info{};
    retro_get_system_info(&info);

    retro_game_info game{};
    game.path = g_config.game_path.c_str();
    if (!retro_load_game(&game)) {
        if (g_host.log != nullptr) g_host.log(g_host.context, 0, "retro_load_game failed");
        retro_deinit();
        return -2;
    }
    g_loaded = true;
    return 0;
}

void runFrame() {
    if (!g_loaded) return;
    retro_run();
}

void stopEngine() {
    if (g_loaded) {
        retro_unload_game();
        g_loaded = false;
    }
    g_frame.clear();
    g_width = 0;
    g_height = 0;
    retro_deinit();
}

} // namespace aurora::psp

#endif /* __has_include(libretro.h) */
