// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef AURORA_WITH_KRKR

#include "krkr_engine.h"

namespace aurora {
namespace krkr {

bool engineAvailable() { return false; }
bool create(const Config&, std::string& error) { error = "this build has no KiriKiri engine"; return false; }
bool openGame(const std::string&, std::string&) { return false; }
bool tick(uint32_t, Frame&, std::string&) { return false; }
void setSurfaceSize(uint32_t, uint32_t) {}
void destroy() {}

} // namespace krkr
} // namespace aurora

#endif
