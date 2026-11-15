// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#include <cstdint>
#include <string>

/* The PPSSPP libretro core has a very different shape from the visual novel
 * engines: it is a single-threaded run loop that pushes finished RGBA frames
 * through a callback and pulls input on demand.
 *
 * This header is the seam between that core and the Aurora module ABI. The
 * real implementation lives in psp_engine_libretro.cpp and only compiles when
 * the core is linked; psp_engine_stub.cpp keeps the app buildable without it. */
namespace aurora::psp {

struct Config {
    std::string game_path;        /* .iso / .cso / .chd */
    std::string save_directory;   /* memory stick directory */
    std::string system_directory; /* PSP firmware directory */
};

struct Host {
    void* context = nullptr;
    /* One finished frame, RGBA8888, top-left origin. */
    void (*frame)(void* context, const void* pixels, int width, int height, int stride_bytes);
    void (*log)(void* context, int level, const char* message);
    int (*button)(void* context, unsigned id); /* 1 while the button is held */
};

bool engineAvailable();

/* Returns 0 on success, a negative code otherwise. */
int startEngine(const Config& config, const Host& host);
void runFrame();
void stopEngine();

/* Names/labels the UI can show even before a game is loaded. */
const char* coreName();

} // namespace aurora::psp
