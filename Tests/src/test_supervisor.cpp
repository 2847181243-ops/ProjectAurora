#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

#include "aurora/supervisor.h"
#include "test_env.h"
#include "test_harness.h"

using namespace aurora;

AURORA_TEST(supervisor_runs_a_module_and_cleans_up) {
    test::Environment env("supervisor-run");
    env.host.setDefaultSurface(320, 240);
    std::string error;

    const AuroraGameInfo game = test::gameInfo("/library/demo.sim", "sim");
    CHECK_EQ(env.supervisor.start("com.aurora.runtime.simulated", game, error), AURORA_OK);
    CHECK(env.supervisor.isRunning("com.aurora.runtime.simulated"));

    for (int i = 0; i < 10; ++i) {
        CHECK_EQ(env.supervisor.tick("com.aurora.runtime.simulated", 1.0 / 60.0, error), AURORA_OK);
    }
    const RuntimeInstance* instance = env.supervisor.instance("com.aurora.runtime.simulated");
    CHECK(instance != nullptr);
    if (instance != nullptr) {
        CHECK_EQ(instance->status.frames_presented, static_cast<std::int64_t>(10));
        CHECK(instance->binding != nullptr);
        if (instance->binding != nullptr) {
            /* The simulated module paints a moving bar in a fixed colour, so a
             * frame that was really rendered can be told apart from a blank one. */
            const std::vector<std::uint8_t>& pixels = instance->binding->surface_pixels;
            bool painted = false;
            for (std::size_t i = 0; i + 3 < pixels.size(); i += 4) {
                if (pixels[i] == 255 && pixels[i + 1] == 210) { painted = true; break; }
            }
            CHECK(painted);
        }
    }

    CHECK_EQ(env.supervisor.stop("com.aurora.runtime.simulated", error), AURORA_OK);
    CHECK(!env.supervisor.isRunning("com.aurora.runtime.simulated"));
    const ModuleEntry* entry = env.registry.find("com.aurora.runtime.simulated");
    CHECK(entry != nullptr);
    if (entry != nullptr) CHECK_EQ(entry->state, AURORA_STATE_READY);
}

AURORA_TEST(supervisor_writes_only_into_the_modules_own_buckets) {
    test::Environment env("supervisor-storage");
    std::string error;
    const AuroraGameInfo game = test::gameInfo("/library/demo.sim", "sim");
    CHECK_EQ(env.supervisor.start("com.aurora.runtime.simulated", game, error), AURORA_OK);
    CHECK_EQ(env.supervisor.tick("com.aurora.runtime.simulated", 1.0 / 60.0, error), AURORA_OK);
    CHECK_EQ(env.supervisor.stop("com.aurora.runtime.simulated", error), AURORA_OK);

    const std::filesystem::path base(env.root);
    CHECK(std::filesystem::exists(base / "RuntimeModules" / "com.aurora.runtime.simulated" / "Config" / "settings.json"));
    CHECK(std::filesystem::exists(base / "RuntimeModules" / "com.aurora.runtime.simulated" / "Saves" / "save.slot1"));
    CHECK(std::filesystem::exists(base / "RuntimeModules" / "com.aurora.runtime.simulated" / "States" / "state.slot1"));
    CHECK(!std::filesystem::exists(base / "RuntimeModules" / "com.aurora.runtime.empty" / "Saves" / "save.slot1"));
}

AURORA_TEST(supervisor_refuses_to_start_a_module_that_is_not_ready) {
    test::Environment env("supervisor-not-ready");
    std::string error;
    const AuroraGameInfo game = test::gameInfo("/library/demo.iso", "iso");
    const AuroraStatus status = env.supervisor.start("com.aurora.runtime.ppsspp", game, error);
    CHECK_EQ(status, AURORA_E_STATE);
    CHECK_CONTAINS(error, "DISCOVERED");
}

AURORA_TEST(supervisor_reports_pause_and_resume) {
    test::Environment env("supervisor-pause");
    std::string error;
    const AuroraGameInfo game = test::gameInfo("/library/demo.sim", "sim");
    CHECK_EQ(env.supervisor.start("com.aurora.runtime.simulated", game, error), AURORA_OK);
    CHECK_EQ(env.supervisor.pause("com.aurora.runtime.simulated", error), AURORA_OK);
    CHECK_EQ(env.supervisor.resume("com.aurora.runtime.simulated", error), AURORA_OK);
    CHECK_EQ(env.supervisor.stop("com.aurora.runtime.simulated", error), AURORA_OK);
}
