// SPDX-License-Identifier: GPL-3.0-or-later
/* Used when the build does not link ONScripter. Keeps the module honest: it can
 * register and be inspected, but starting it reports UNSUPPORTED. */
#ifndef AURORA_WITH_ONSCRIPTER

#include "ons_engine.h"

namespace aurora {
namespace ons {

bool engineAvailable() { return false; }

int runBlocking(const Config&) { return -1; }

void requestStop() {}

} // namespace ons
} // namespace aurora

#endif
