// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef AURORA_ONS_ENGINE_H
#define AURORA_ONS_ENGINE_H

#include <string>
#include <vector>

/* The seam between Aurora and ONScripter.
 *
 * ONScripter owns a blocking main loop that stops inside commands, so the module
 * hands it a thread and only keeps the lifecycle. The split matters on iOS: SDL
 * must be initialised on the main thread, while the loop can run anywhere.
 */
namespace aurora {
namespace ons {

struct Config {
    std::string game_directory;
    std::string save_directory;
    std::string log_directory;
};

/* False when this build was compiled without the engine. */
bool engineAvailable();

/* Main thread: opens the script and brings up SDL. Returns 0 on success. */
int initEngine(const Config& config);

/* Worker thread: runs the script until it ends or requestStop() is called. */
int runLoop();

/* Any thread: asks the event pump to return. */
/* kind: 0 = down, 1 = move, 2 = up. x and y are video-surface pixels. */
void pushPointer(int kind, int x, int y);

void requestStop();

/* Human readable description of what the video backend gave us; empty when the
 * engine could not report it. */
/* Copies the current video surface into RGBA8. Returns false when the engine has
 * not produced a surface yet (or the backend has no frame source at all). */
bool grabFrame(std::vector<uint8_t>& rgba, uint32_t& width, uint32_t& height);

std::string describeVideo();

/* Main thread: tears SDL down after the loop returned. */
void shutdownEngine();

} // namespace ons
} // namespace aurora

#endif
