// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef AURORA_WITH_ONSCRIPTER

#include "ons_engine.h"

namespace aurora {
namespace ons {

bool engineAvailable() { return false; }
int initEngine(const Config&) { return -1; }
int runLoop() { return -1; }
void requestStop() {}
void shutdownEngine() {}

} // namespace ons
} // namespace aurora

#endif
