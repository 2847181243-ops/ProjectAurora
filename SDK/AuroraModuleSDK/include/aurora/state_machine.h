// SPDX-FileCopyrightText: 2026 Project Aurora contributors
// SPDX-License-Identifier: GPL-3.0-or-later

/* Module state machine.
 *
 * The states answer different questions and the platform must not blur them:
 *   DISCOVERED  manifest known, no usable binary
 *   BUILT       binary linked in, at least one precondition unmet
 *   READY       every precondition met, a game can start
 *   RUNNING     an instance is live
 *   DISABLED    the user turned it off
 *   FAILED      a probe or a run failed
 * States move only along the edges below. Anything else is a bug, and the
 * registry refuses it instead of guessing.
 */
#ifndef AURORA_STATE_MACHINE_H
#define AURORA_STATE_MACHINE_H

#include <vector>

#include "aurora/module_api.h"

namespace aurora {

bool isTransitionAllowed(AuroraModuleState from, AuroraModuleState to);

/* Applies a transition, returning false and leaving |state| untouched when the
 * edge is illegal. Every rejection is evidence, not a silent no-op. */
bool applyTransition(AuroraModuleState& state, AuroraModuleState to);

/* Shortest legal path between two states, |from| excluded, |to| included.
 * Empty when no path exists. Derived states are recomputed constantly, so the
 * registry needs a route rather than a single edge. */
std::vector<AuroraModuleState> transitionPath(AuroraModuleState from, AuroraModuleState to);

} // namespace aurora

#endif
