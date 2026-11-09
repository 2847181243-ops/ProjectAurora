/* SPDX-License-Identifier: GPL-3.0-or-later
 *
 * sdl12-compat declares one hook as external and expects the application to
 * provide it: it uses it to report that it failed to load SDL2 at runtime. We
 * link SDL2 statically, so the path is never taken, but the reference still has
 * to resolve at link time.
 */
#include <stdio.h>

void error_dialog(const char *errorMsg)
{
    if (errorMsg != NULL) {
        fprintf(stderr, "[sdl12-compat] %s\n", errorMsg);
    }
}
