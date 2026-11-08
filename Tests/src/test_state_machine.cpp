#include <vector>

#include "aurora/state_machine.h"
#include "test_harness.h"

using namespace aurora;

AURORA_TEST(state_machine_allows_the_declared_edges) {
    CHECK(isTransitionAllowed(AURORA_STATE_DISCOVERED, AURORA_STATE_BUILT));
    CHECK(isTransitionAllowed(AURORA_STATE_BUILT, AURORA_STATE_READY));
    CHECK(isTransitionAllowed(AURORA_STATE_READY, AURORA_STATE_RUNNING));
    CHECK(isTransitionAllowed(AURORA_STATE_RUNNING, AURORA_STATE_FAILED));
    CHECK(isTransitionAllowed(AURORA_STATE_FAILED, AURORA_STATE_BUILT));
}

AURORA_TEST(state_machine_refuses_nonsense_edges) {
    CHECK(!isTransitionAllowed(AURORA_STATE_DISCOVERED, AURORA_STATE_RUNNING));
    CHECK(isTransitionAllowed(AURORA_STATE_READY, AURORA_STATE_DEPRECATED));
    CHECK(!isTransitionAllowed(AURORA_STATE_BUILDING, AURORA_STATE_RUNNING));
    CHECK(!isTransitionAllowed(AURORA_STATE_DISCOVERED, AURORA_STATE_READY));
    AuroraModuleState state = AURORA_STATE_DISCOVERED;
    CHECK(!applyTransition(state, AURORA_STATE_RUNNING));
    CHECK_EQ(state, AURORA_STATE_DISCOVERED);
}

AURORA_TEST(state_machine_finds_a_path_between_states) {
    const std::vector<AuroraModuleState> to_ready =
        transitionPath(AURORA_STATE_DISCOVERED, AURORA_STATE_READY);
    CHECK_EQ(to_ready.size(), static_cast<std::size_t>(2));
    if (to_ready.size() == 2) {
        CHECK_EQ(to_ready[0], AURORA_STATE_BUILT);
        CHECK_EQ(to_ready[1], AURORA_STATE_READY);
    }

    const std::vector<AuroraModuleState> to_disabled =
        transitionPath(AURORA_STATE_READY, AURORA_STATE_DISABLED);
    CHECK(!to_disabled.empty());
    if (!to_disabled.empty()) CHECK_EQ(to_disabled.back(), AURORA_STATE_DISABLED);

    CHECK(transitionPath(AURORA_STATE_READY, AURORA_STATE_READY).empty());
}
