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
bool openGame(const std::string& game_root, std::string& error);
/* Advances the engine and leaves the current frame in out until the next call. */
bool tick(uint32_t delta_ms, Frame& out, std::string& error);
void setSurfaceSize(uint32_t width, uint32_t height);
void destroy();

} // namespace krkr
} // namespace aurora

#endif
