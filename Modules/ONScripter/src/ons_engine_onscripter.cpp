// SPDX-License-Identifier: GPL-3.0-or-later
/* The real side of the ONScripter seam.
 *
 * Only compiled when AURORA_WITH_ONSCRIPTER is defined and the engine sources are
 * on the include path. ONScripterLabel owns a blocking loop, so this runs on the
 * thread the module created and never returns until the script ends or a quit
 * event arrives.
 */
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

/* ONScripter reads events from the SDL queue, so a quit request is just an event. */
void pushQuit() {
    SDL_Event event;
    memset(&event, 0, sizeof(event));
    event.type = SDL_QUIT;
    SDL_PushEvent(&event);
}

} // namespace

bool engineAvailable() { return true; }

int runBlocking(const Config& config) {
    g_quit_requested.store(false);

    ONScripterLabel engine;
    engine.setArchivePath(config.game_directory.c_str());
    engine.setSavePath(config.save_directory.c_str());

    if (engine.init() != 0) {
        return 2; /* initialisation failed: missing script, bad archive, ... */
    }

    engine.executeLabel();
    pushQuit();
    SDL_Quit();
    return 0;
}

void requestStop() {
    g_quit_requested.store(true);
    pushQuit();
}

} // namespace ons
} // namespace aurora

#endif /* AURORA_WITH_ONSCRIPTER */
