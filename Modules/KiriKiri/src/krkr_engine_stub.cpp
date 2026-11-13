// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef AURORA_WITH_KRKR

#include "krkr_engine.h"

namespace aurora {
namespace krkr {

bool engineAvailable() { return false; }
bool create(const Config&, std::string& error) { error = "this build has no KiriKiri engine"; return false; }
bool openGame(const std::string&, std::string&) { return false; }
bool tick(uint32_t, Frame&, std::string&) { return false; }
void sendPointer(int, float, float) {}
void setSurfaceSize(uint32_t, uint32_t) {}
bool pause() { return false; }
bool resume() { return false; }
void destroy() {}

} // namespace krkr
} // namespace aurora

#endif
