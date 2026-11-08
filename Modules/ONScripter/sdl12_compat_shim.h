/* SPDX-License-Identifier: GPL-3.0-or-later
 *
 * sdl12-compat's compatibility headers are deliberately minimal: they cover the
 * SDL 1.2 API, not every SDL 1.2 typedef or calling convention the old libraries
 * expect. SMPEG (and parts of ONScripter) were written against the full SDL 1.2
 * headers, so this shim fills the small gaps.
 */
#ifndef AURORA_SDL12_COMPAT_SHIM_H
#define AURORA_SDL12_COMPAT_SHIM_H

#include <SDL.h>

/* SDL 1.2 defines this in SDL_audio.h; the shim headers leave it out. */
#ifndef AURORA_SDL_AUDIOFORMAT_DEFINED
typedef Uint16 SDL_AudioFormat;
#define AURORA_SDL_AUDIOFORMAT_DEFINED 1
#endif

/* SDL 1.2's thread entry takes two arguments; the shim headers expose the
 * three argument form. Old sources keep calling the two argument version. */
#define SDL_CreateThread(fn, data) SDL_CreateThread(fn, "aurora", data)

#endif /* AURORA_SDL12_COMPAT_SHIM_H */
