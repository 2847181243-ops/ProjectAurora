#include <string>

#include "aurora/supervisor.h"
#include "test_env.h"
#include "test_harness.h"

using namespace aurora;

namespace {
const char* kFaultInjector = "com.aurora.runtime.faultinjector";
const char* kSimulated = "com.aurora.runtime.simulated";
} // namespace

AURORA_TEST(fault_rejected_game_is_a_user_error_not_a_module_failure) {
    test::Environment env("fault-unsupported");
    std::string error;
    const AuroraGameInfo game = test::gameInfo("/library/odd.bin", "unsupported");
    CHECK_EQ(env.supervisor.start(kFaultInjector, game, error), AURORA_E_UNSUPPORTED_GAME);
    CHECK(!env.supervisor.isRunning(kFaultInjector));

    const ModuleEntry* entry = env.registry.find(kFaultInjector);
    CHECK(entry != nullptr);
    if (entry != nullptr) {
        /* Rejecting one game must not poison the module for the next one. */
        CHECK_EQ(entry->state, AURORA_STATE_READY);
        CHECK(entry->last_error.empty());
    }
}

AURORA_TEST(fault_exception_across_the_abi_is_caught) {
    test::Environment env("fault-throw");
    std::string error;
    const AuroraGameInfo game = test::gameInfo("/library/crash.bin", "crash");
    const AuroraStatus status = env.supervisor.start(kFaultInjector, game, error);
    CHECK_EQ(status, AURORA_E_INTERNAL);
    CHECK(env.supervisor.isRunning(kFaultInjector) == false);

    const ModuleEntry* entry = env.registry.find(kFaultInjector);
    CHECK(entry != nullptr);
    if (entry != nullptr) {
        CHECK_EQ(entry->state, AURORA_STATE_FAILED);
        CHECK_CONTAINS(entry->last_error, "load_game failed");
    }
}

AURORA_TEST(fault_a_failed_module_leaves_the_others_working) {
    test::Environment env("fault-isolated");
    env.host.setDefaultSurface(160, 120);
    std::string error;

    const AuroraGameInfo crash = test::gameInfo("/library/crash.bin", "crash");
    CHECK_EQ(env.supervisor.start(kFaultInjector, crash, error), AURORA_E_INTERNAL);

    const AuroraGameInfo healthy = test::gameInfo("/library/demo.sim", "sim");
    CHECK_EQ(env.supervisor.start(kSimulated, healthy, error), AURORA_OK);
    CHECK_EQ(env.supervisor.tick(kSimulated, 1.0 / 60.0, error), AURORA_OK);
    CHECK_EQ(env.supervisor.stop(kSimulated, error), AURORA_OK);

    /* And the same registry still lists the broken one as broken, not as ready. */
    const ModuleEntry* broken = env.registry.find(kFaultInjector);
    CHECK(broken != nullptr);
    if (broken != nullptr) CHECK_EQ(broken->state, AURORA_STATE_FAILED);
    CHECK_EQ(env.registry.countInState(AURORA_STATE_FAILED), static_cast<std::size_t>(1));
}

AURORA_TEST(fault_failed_state_is_sticky_until_reset) {
    test::Environment env("fault-sticky");
    std::string error;
    const AuroraGameInfo fail_start = test::gameInfo("/library/boom.bin", "failstart");
    CHECK_EQ(env.supervisor.start(kFaultInjector, fail_start, error), AURORA_E_INIT_FAILED);

    const ModuleEntry* entry = env.registry.find(kFaultInjector);
    CHECK(entry != nullptr);
    if (entry != nullptr) CHECK_EQ(entry->state, AURORA_STATE_FAILED);

    env.registry.evaluateAll();
    entry = env.registry.find(kFaultInjector);
    if (entry != nullptr) CHECK_EQ(entry->state, AURORA_STATE_FAILED);

    CHECK(env.supervisor.resetFailure(kFaultInjector, error));
    entry = env.registry.find(kFaultInjector);
    if (entry != nullptr) CHECK_EQ(entry->state, AURORA_STATE_READY);

    const AuroraGameInfo healthy = test::gameInfo("/library/demo.bin", "ok");
    CHECK_EQ(env.supervisor.start(kFaultInjector, healthy, error), AURORA_OK);
    CHECK_EQ(env.supervisor.stop(kFaultInjector, error), AURORA_OK);
}

AURORA_TEST(fault_stop_failure_still_releases_the_instance) {
    test::Environment env("fault-stop");
    std::string error;
    const AuroraGameInfo game = test::gameInfo("/library/demo.bin", "stopfail");
    CHECK_EQ(env.supervisor.start(kFaultInjector, game, error), AURORA_OK);
    CHECK_EQ(env.supervisor.stop(kFaultInjector, error), AURORA_E_IO);
    CHECK(!env.supervisor.isRunning(kFaultInjector));
    const ModuleEntry* entry = env.registry.find(kFaultInjector);
    CHECK(entry != nullptr);
    if (entry != nullptr) {
        /* The instance is gone and the module can be started again. */
        CHECK_EQ(entry->state, AURORA_STATE_READY);
    }
}

AURORA_TEST(fault_tick_failure_leaves_the_module_restartable) {
    test::Environment env("fault-tick");
    std::string error;
    const AuroraGameInfo game = test::gameInfo("/library/demo.bin", "tickfail");
    CHECK_EQ(env.supervisor.start(kFaultInjector, game, error), AURORA_OK);
    CHECK_EQ(env.supervisor.tick(kFaultInjector, 1.0 / 60.0, error), AURORA_E_IO);
    CHECK(!env.supervisor.isRunning(kFaultInjector));

    /* Regression: the entry used to stay RUNNING with no instance, which made
     * start(), stop() and setEnabled() all refuse. */
    const ModuleEntry* entry = env.registry.find(kFaultInjector);
    CHECK(entry != nullptr);
    if (entry != nullptr) CHECK_EQ(entry->state, AURORA_STATE_READY);

    const AuroraGameInfo healthy = test::gameInfo("/library/demo.bin", "ok");
    CHECK_EQ(env.supervisor.start(kFaultInjector, healthy, error), AURORA_OK);
    CHECK_EQ(env.supervisor.stop(kFaultInjector, error), AURORA_OK);
}
