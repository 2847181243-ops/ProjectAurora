// SPDX-FileCopyrightText: 2026 Project Aurora contributors
// SPDX-License-Identifier: GPL-3.0-or-later

#include <cstdint>
#include <string>

#include "aurora/supervisor.h"
#include "test_env.h"
#include "test_harness.h"

using namespace aurora;

/* Starting and stopping a module repeatedly is what a session does every time
 * somebody switches games. If the supervisor leaks an instance or leaves a
 * binding behind, that only shows up after enough rounds, so do enough rounds. */
AURORA_TEST(supervisor_survives_many_start_stop_cycles) {
    test::Environment env("supervisor-soak");
    env.host.setDefaultSurface(320, 240);
    std::string error;

    const AuroraGameInfo game = test::gameInfo("/library/demo.sim", "sim");
    const char* id = "com.aurora.runtime.simulated";

    for (int round = 0; round < 25; ++round) {
        CHECK_EQ(env.supervisor.start(id, game, error), AURORA_OK);
        CHECK(env.supervisor.isRunning(id));

        for (int i = 0; i < 3; ++i) {
            CHECK_EQ(env.supervisor.tick(id, 1.0 / 60.0, error), AURORA_OK);
        }

        const RuntimeInstance* instance = env.supervisor.instance(id);
        CHECK(instance != nullptr);
        if (instance != nullptr) {
            CHECK(instance->binding != nullptr);
        }

        CHECK_EQ(env.supervisor.stop(id, error), AURORA_OK);
        CHECK(!env.supervisor.isRunning(id));
        CHECK(env.supervisor.instance(id) == nullptr);
    }
}

/* A module that fails to start must leave nothing running, so the next attempt
 * sees a clean slate instead of a half-built instance. */
AURORA_TEST(supervisor_recovers_after_a_failed_start) {
    test::Environment env("supervisor-recover");
    env.host.setDefaultSurface(320, 240);
    std::string error;

    /* An unknown module is the cleanest way to get a real start failure: the
     * registry has no entry, so the supervisor must reject it and stay usable. */
    const AuroraGameInfo game = test::gameInfo("/library/demo.sim", "sim");
    const char* id = "com.aurora.runtime.simulated";

    const AuroraStatus failed = env.supervisor.start("com.aurora.runtime.does-not-exist", game, error);
    CHECK(failed != AURORA_OK);
    CHECK(!env.supervisor.isRunning("com.aurora.runtime.does-not-exist"));

    CHECK_EQ(env.supervisor.start(id, game, error), AURORA_OK);
    CHECK(env.supervisor.isRunning(id));
    CHECK_EQ(env.supervisor.stop(id, error), AURORA_OK);
}
