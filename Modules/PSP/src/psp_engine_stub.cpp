// SPDX-License-Identifier: GPL-3.0-or-later
#include "psp_engine.h"

#if !__has_include("libretro.h")

/* Built when the PPSSPP core is not part of the link. The module still shows up
 * in the library, says it cannot start, and the rest of the app is unaffected. */
namespace aurora::psp {

bool engineAvailable() { return false; }

int startEngine(const Config&, const Host&) { return -1; }

void runFrame() {}

void stopEngine() {}

const char* coreName() { return "PPSSPP core not linked"; }

} // namespace aurora::psp

#endif /* !__has_include(libretro.h) */
