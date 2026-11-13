// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef AURORA_KRKR_ENGINE_H
#define AURORA_KRKR_ENGINE_H

#include <cstdint>
#include <string>

/* The seam between Aurora and KiriKiri2 (KrKr2-Next).
 *
 * Unlike ONScripter this engine is tick driven: the host advances it by a delta
 * and then reads an RGBA frame, which maps directly onto the Aurora ABI.
 */
namespace aurora {
namespace krkr {

struct Config {
    std::string game_root;
    std::string writable_directory;
    std::string cache_directory;
};

struct Frame {
    const uint8_t* pixels = nullptr;
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t stride_bytes = 0;
};

bool engineAvailable();
bool create(const Config& config, std::string& error);
bool sameGameOpen(const std::string& game_root);
bool openGame(const std::string& game_root, std::string& error);
/* Advances the engine and leaves the current frame in out until the next call. */
bool tick(uint32_t delta_ms, Frame& out, std::string& error);
/* x and y are in engine-surface pixels. kind: 0 = down, 1 = move, 2 = up. */
void sendPointer(int kind, float x, float y);

void setSurfaceSize(uint32_t width, uint32_t height);
bool pause();
bool resume();

void destroy();

} // namespace krkr
} // namespace aurora

#endif
