#include "aurora/state_machine.h"

#include <deque>
#include <map>
#include <set>

namespace aurora {

namespace {

const std::vector<std::pair<AuroraModuleState, AuroraModuleState>>& edges() {
    static const std::vector<std::pair<AuroraModuleState, AuroraModuleState>> table = {
        {AURORA_STATE_DISCOVERED, AURORA_STATE_PLANNED},
        {AURORA_STATE_DISCOVERED, AURORA_STATE_BUILT},
        {AURORA_STATE_DISCOVERED, AURORA_STATE_DISABLED},
        {AURORA_STATE_DISCOVERED, AURORA_STATE_DEPRECATED},

        {AURORA_STATE_PLANNED, AURORA_STATE_BUILDING},
        {AURORA_STATE_PLANNED, AURORA_STATE_BUILT},
        {AURORA_STATE_PLANNED, AURORA_STATE_DISABLED},
        {AURORA_STATE_PLANNED, AURORA_STATE_DEPRECATED},

        {AURORA_STATE_BUILDING, AURORA_STATE_BUILT},
        {AURORA_STATE_BUILDING, AURORA_STATE_FAILED},

        {AURORA_STATE_BUILT, AURORA_STATE_READY},
        {AURORA_STATE_BUILT, AURORA_STATE_DISCOVERED},
        {AURORA_STATE_BUILT, AURORA_STATE_DISABLED},
        {AURORA_STATE_BUILT, AURORA_STATE_FAILED},
        {AURORA_STATE_BUILT, AURORA_STATE_DEPRECATED},

        {AURORA_STATE_READY, AURORA_STATE_RUNNING},
        {AURORA_STATE_READY, AURORA_STATE_BUILT},
        {AURORA_STATE_READY, AURORA_STATE_DISABLED},
        {AURORA_STATE_READY, AURORA_STATE_FAILED},
        {AURORA_STATE_READY, AURORA_STATE_DEPRECATED},

        {AURORA_STATE_RUNNING, AURORA_STATE_READY},
        {AURORA_STATE_RUNNING, AURORA_STATE_BUILT},
        {AURORA_STATE_RUNNING, AURORA_STATE_FAILED},

        {AURORA_STATE_DISABLED, AURORA_STATE_BUILT},
        {AURORA_STATE_DISABLED, AURORA_STATE_DISCOVERED},
        {AURORA_STATE_DISABLED, AURORA_STATE_DEPRECATED},

        {AURORA_STATE_FAILED, AURORA_STATE_BUILT},
        {AURORA_STATE_FAILED, AURORA_STATE_DISCOVERED},
        {AURORA_STATE_FAILED, AURORA_STATE_DISABLED},
        {AURORA_STATE_FAILED, AURORA_STATE_DEPRECATED},

        {AURORA_STATE_DEPRECATED, AURORA_STATE_DISABLED},
    };
    return table;
}

} // namespace

bool isTransitionAllowed(AuroraModuleState from, AuroraModuleState to) {
    for (const auto& edge : edges()) {
        if (edge.first == from && edge.second == to) return true;
    }
    return false;
}

bool applyTransition(AuroraModuleState& state, AuroraModuleState to) {
    if (!isTransitionAllowed(state, to)) return false;
    state = to;
    return true;
}

std::vector<AuroraModuleState> transitionPath(AuroraModuleState from, AuroraModuleState to) {
    std::vector<AuroraModuleState> path;
    if (from == to) return path;

    std::map<AuroraModuleState, AuroraModuleState> parent;
    std::set<AuroraModuleState> visited;
    std::deque<AuroraModuleState> queue;
    queue.push_back(from);
    visited.insert(from);
    bool found = false;

    while (!queue.empty() && !found) {
        const AuroraModuleState current = queue.front();
        queue.pop_front();
        for (const auto& edge : edges()) {
            if (edge.first != current) continue;
            if (visited.count(edge.second) != 0) continue;
            visited.insert(edge.second);
            parent[edge.second] = current;
            if (edge.second == to) { found = true; break; }
            queue.push_back(edge.second);
        }
    }
    if (!found) return path;

    for (AuroraModuleState cursor = to; cursor != from; cursor = parent[cursor]) {
        path.push_back(cursor);
    }
    std::reverse(path.begin(), path.end());
    return path;
}

} // namespace aurora
