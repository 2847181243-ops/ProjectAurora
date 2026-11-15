// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef AURORA_WITH_ONSCRIPTER

#include "ons_engine.h"

namespace aurora {
namespace ons {

bool engineAvailable() { return false; }
int initEngine(const Config&) { return -1; }
int runLoop() { return -1; }
void pushPointer(int, int, int) {}
bool screenSize(int& width, int& height) { width = 0; height = 0; return false; }
void requestStop() {}
void shutdownEngine() {}
bool grabFrame(std::vector<uint8_t>&, uint32_t&, uint32_t&) { return false; }

std::string describeVideo() { return ""; }

} // namespace ons
} // namespace aurora

#endif
