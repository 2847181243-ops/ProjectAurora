// SPDX-License-Identifier: GPL-3.0-or-later
/* The real side of the ONScripter seam. */
#if defined(AURORA_WITH_ONSCRIPTER)

#include <atomic>
#include <cstdio>
#include <cstring>

#include <SDL.h>

#include "ONScripterLabel.h"
#include "ons_engine.h"

/* sdl12-compat's headers only cover the SDL 1.2 API, so declare the SDL2 entry
 * point we need ourselves; the symbol comes from the SDL2 we link. */
extern "C" void SDL_SetMainReady(void);

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
    /* SDL2 refuses to start when main() is not its own wrapper; a host app that
     * embeds the engine has to say so explicitly. */
    SDL_SetMainReady();
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
    /* The engine reports fatal errors by calling exit(), which tears SDL down on
     * this thread and trips a UIKit assertion. The build patches those calls into
     * a throw, so a bad script ends the loop instead of the process. */
    try {
        g_engine->executeLabel();
    } catch (int code) {
        std::fprintf(stderr, "[onscripter] engine aborted with code %d\n", code);
        return code == 0 ? 0 : code;
    } catch (...) {
        std::fprintf(stderr, "[onscripter] engine threw an unknown exception\n");
        return 4;
    }
    return 0;
}

void pushPointer(int kind, int x, int y) {
    /* ONScripter reads SDL 1.2 events, so touch arrives as a left-button mouse. */
    SDL_Event event;
    std::memset(&event, 0, sizeof(event));
    if (kind == 1) {
        event.type = SDL_MOUSEMOTION;
        event.motion.x = static_cast<Uint16>(x);
        event.motion.y = static_cast<Uint16>(y);
    } else {
        event.type = kind == 0 ? SDL_MOUSEBUTTONDOWN : SDL_MOUSEBUTTONUP;
        event.button.button = SDL_BUTTON_LEFT;
        event.button.x = static_cast<Uint16>(x);
        event.button.y = static_cast<Uint16>(y);
    }
    SDL_PushEvent(&event);
}

void requestStop() {
    g_quit_requested.store(true);
    pushQuit();
}

bool grabFrame(std::vector<uint8_t>& rgba, uint32_t& width, uint32_t& height) {
    SDL_Surface* surface = SDL_GetVideoSurface();
    if (surface == nullptr || surface->pixels == nullptr || surface->w <= 0 || surface->h <= 0) return false;

    /* The engine draws on its own thread, so a frame can tear while we copy it.
     * A tearing frame is acceptable here; blocking the engine is not. */
    if (SDL_MUSTLOCK(surface) && SDL_LockSurface(surface) != 0) return false;

    const int w = surface->w;
    const int h = surface->h;
    const int bpp = surface->format->BytesPerPixel;
    rgba.resize(static_cast<std::size_t>(w) * static_cast<std::size_t>(h) * 4u);

    const SDL_PixelFormat* format = surface->format;
    const uint8_t* base = static_cast<const uint8_t*>(surface->pixels);
    for (int y = 0; y < h; ++y) {
        const uint8_t* row = base + static_cast<std::size_t>(y) * surface->pitch;
        uint8_t* dst = rgba.data() + static_cast<std::size_t>(y) * static_cast<std::size_t>(w) * 4u;
        for (int x = 0; x < w; ++x) {
            uint32_t value = 0;
            switch (bpp) {
                case 1: value = row[x]; break;
                case 2: value = reinterpret_cast<const uint16_t*>(row)[x]; break;
                case 3: {
                    const uint8_t* p = row + x * 3;
                    value = static_cast<uint32_t>(p[0]) | (static_cast<uint32_t>(p[1]) << 8) |
                            (static_cast<uint32_t>(p[2]) << 16);
                    break;
                }
                default: value = reinterpret_cast<const uint32_t*>(row)[x]; break;
            }
            auto channel = [&](uint32_t mask, uint8_t shift, uint8_t loss) -> uint8_t {
                if (mask == 0) return 0xFF;
                const uint32_t raw = (value & mask) >> shift;
                return static_cast<uint8_t>(raw << loss);
            };
            dst[x * 4 + 0] = channel(format->Rmask, format->Rshift, format->Rloss);
            dst[x * 4 + 1] = channel(format->Gmask, format->Gshift, format->Gloss);
            dst[x * 4 + 2] = channel(format->Bmask, format->Bshift, format->Bloss);
            dst[x * 4 + 3] = 0xFF;
        }
    }
    if (SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);

    width = static_cast<uint32_t>(w);
    height = static_cast<uint32_t>(h);
    return true;
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
