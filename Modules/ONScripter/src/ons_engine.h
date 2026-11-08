// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef AURORA_ONS_ENGINE_H
#define AURORA_ONS_ENGINE_H

#include <string>

/* The seam between Aurora and ONScripter.
 *
 * ONScripter owns a blocking main loop that stops inside commands, so the module
 * hands it a thread and only keeps the lifecycle. Everything engine specific lives
 * behind this interface, which keeps the module itself free of engine headers.
 */
namespace aurora {
namespace ons {

struct Config {
    std::string game_directory;  /* where the .txt / .nsa / .sar files live */
    std::string save_directory;  /* writable, module-owned */
    std::string log_directory;
};

/* False when this build was compiled without the engine (AURORA_WITH_ONSCRIPTER
 * unset). The module then reports UNSUPPORTED instead of pretending. */
bool engineAvailable();

/* Blocks until the script ends or requestStop() is called. Returns 0 on a clean
 * exit, non-zero when the engine failed to initialise or crashed out. */
int runBlocking(const Config& config);

/* Safe to call from any thread; makes the next event pump return. */
void requestStop();

} // namespace ons
} // namespace aurora

#endif
