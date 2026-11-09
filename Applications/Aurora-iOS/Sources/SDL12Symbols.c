/* SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Two hooks sdl12-compat expects the application to provide.
 *
 * 1. error_dialog: reports that it failed to load SDL2 at runtime. We link SDL2
 *    statically, so the path is never taken, but the reference must resolve.
 * 2. SDL12COMPAT_NSLog: normally lives in the Cocoa glue, which is not built for
 *    iOS. Plain stderr logging keeps the behaviour observable.
 */
#include <stddef.h>
#include <stdio.h>

void error_dialog(const char *errorMsg)
{
    if (errorMsg != NULL) {
        fprintf(stderr, "[sdl12-compat] %s\n", errorMsg);
    }
}

void SDL12COMPAT_NSLog(const char *prefix, const char *text)
{
    fprintf(stderr, "[sdl12-compat] %s%s\n", prefix != NULL ? prefix : "", text != NULL ? text : "");
}
