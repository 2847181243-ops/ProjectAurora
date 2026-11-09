// SPDX-License-Identifier: GPL-3.0-or-later
/* The real side of the ONScripter seam. */
#if defined(AURORA_WITH_ONSCRIPTER)

#include <atomic>
#include <cstring>

#include <SDL.h>

#include "ONScripterLabel.h"
#include "ons_engine.h"

namespace aurora {
namespace ons {

namespace {

std::atomic<bool> g_quit_requested{false};
ONScripterLabel* g_engine = nullptr;

void pushQuit() {
    SDL_Event event;
    std::memset(&event, 0, sizeof(event));
    event.type = SDL_QUIT;
    SDL_PushEvent(&event);
}

} // namespace

bool engineAvailable() { return true; }

int initEngine(const Config& config) {
    g_quit_requested.store(false);
    if (g_engine == nullptr) {
        g_engine = new ONScripterLabel();
    }
    g_engine->setArchivePath(config.game_directory.c_str());
    g_engine->setSavePath(config.save_directory.c_str());
    /* SDL video must come up on the main thread on iOS, so init happens here and
     * not inside the loop thread. */
    return g_engine->init() == 0 ? 0 : 2;
}

int runLoop() {
    if (g_engine == nullptr) return 3;
    g_engine->executeLabel();
    return 0;
}

void requestStop() {
    g_quit_requested.store(true);
    pushQuit();
}

std::string describeVideo() {
    const char* driver = SDL_VideoDriverName(nullptr, 0);
    char name[64] = {0};
    SDL_VideoDriverName(name, sizeof(name));
    SDL_Surface* surface = SDL_GetVideoSurface();
    std::string text = "video driver: ";
    text += name[0] != 0 ? name : (driver != nullptr ? driver : "unknown");
    if (surface != nullptr) {
        text += " surface: " + std::to_string(surface->w) + "x" + std::to_string(surface->h);
    } else {
        text += " surface: none";
    }
    return text;
}

void shutdownEngine() {
    SDL_Quit();
}

} // namespace ons
} // namespace aurora

#endif /* AURORA_WITH_ONSCRIPTER */
