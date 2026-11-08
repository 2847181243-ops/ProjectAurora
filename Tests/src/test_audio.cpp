// SPDX-License-Identifier: GPL-3.0-or-later
#include <cstdint>
#include <string>

#include "aurora/supervisor.h"
#include "test_env.h"
#include "test_harness.h"

using namespace aurora;

AURORA_TEST(audio_host_reports_its_output_format) {
    test::Environment env("audio-spec");
    HostBinding* binding = env.host.createBinding("com.aurora.runtime.simulated");
    CHECK(binding != nullptr);
    if (binding == nullptr) return;

    AuroraAudioSpec spec{};
    const AuroraStatus status =
        binding->services.audio_spec(binding->services.host_context, &spec);
    CHECK_EQ(status, AURORA_OK);
    CHECK(spec.sample_rate >= 8000u);
    CHECK(spec.channels >= 1u);
    env.host.releaseBinding(binding);
}

AURORA_TEST(audio_submitted_frames_reach_the_host) {
    test::Environment env("audio-submit");
    std::string error;
    const AuroraGameInfo game = test::gameInfo("/library/demo.sim", "sim");
    CHECK_EQ(env.supervisor.start("com.aurora.runtime.simulated", game, error), AURORA_OK);
    for (int i = 0; i < 6; ++i) {
        CHECK_EQ(env.supervisor.tick("com.aurora.runtime.simulated", 1.0 / 60.0, error), AURORA_OK);
    }

    const RuntimeInstance* instance = env.supervisor.instance("com.aurora.runtime.simulated");
    CHECK(instance != nullptr);
    if (instance != nullptr && instance->binding != nullptr) {
        /* 48000 / 60 = 800 frames per tick, six ticks. */
        CHECK_EQ(instance->binding->audio_frames_received,
                 static_cast<std::uint64_t>(4800));
        CHECK(instance->binding->audio_peak > 0.05f);
        CHECK_EQ(instance->binding->audio_spec_seen.sample_rate, 48000u);
        CHECK_EQ(instance->binding->audio_spec_seen.channels, 2u);
    }
    CHECK_EQ(env.supervisor.stop("com.aurora.runtime.simulated", error), AURORA_OK);
}

AURORA_TEST(audio_rejects_malformed_buffers) {
    test::Environment env("audio-invalid");
    HostBinding* binding = env.host.createBinding("com.aurora.runtime.simulated");
    CHECK(binding != nullptr);
    if (binding == nullptr) return;

    float samples[8] = {0};
    AuroraAudioBuffer buffer{};
    buffer.struct_size = sizeof(AuroraAudioBuffer);
    buffer.samples = samples;
    buffer.frame_count = 4;
    buffer.sample_rate = 48000;
    buffer.channels = 2;
    buffer.format = AURORA_AUDIO_F32;

    CHECK_EQ(binding->services.audio_submit(binding->services.host_context, &buffer), AURORA_OK);

    buffer.channels = 0;
    CHECK_EQ(binding->services.audio_submit(binding->services.host_context, &buffer),
             AURORA_E_INVALID_ARGUMENT);

    buffer.channels = 2;
    buffer.samples = nullptr;
    CHECK_EQ(binding->services.audio_submit(binding->services.host_context, &buffer),
             AURORA_E_INVALID_ARGUMENT);

    env.host.releaseBinding(binding);
}
