// SPDX-License-Identifier: GPL-3.0-or-later
#include <string>

#include "aurora/input_profile.h"
#include "test_harness.h"

using namespace aurora;

namespace {

std::string sampleProfile() {
    return R"json({
        "name": "test",
        "bindings": [
            {"action": "a", "pad_mask": 1, "key_code": 90},
            {"action": "start", "touch": {"x": 0.4, "y": 0.9, "w": 0.2, "h": 0.1}}
        ]
    })json";
}

} // namespace

AURORA_TEST(input_profile_parses_bindings) {
    InputProfile profile;
    std::string error;
    CHECK(InputProfile::parse(sampleProfile(), profile, error));
    CHECK_EQ(profile.name, std::string("test"));
    CHECK_EQ(profile.bindings.size(), static_cast<std::size_t>(2));
    if (profile.bindings.size() == 2) {
        CHECK_EQ(profile.bindings[0].action, InputAction::A);
        CHECK_EQ(profile.bindings[0].pad_mask, 1u);
        CHECK_EQ(profile.bindings[1].touch_w, 0.2f);
    }
}

AURORA_TEST(input_profile_rejects_unknown_action) {
    InputProfile profile;
    std::string error;
    const std::string json = R"json({"bindings": [{"action": "teleport"}]})json";
    CHECK(!InputProfile::parse(json, profile, error));
    CHECK_CONTAINS(error, "unknown action");
}

AURORA_TEST(input_maps_gamepad_and_keyboard) {
    InputProfile profile;
    std::string error;
    CHECK(InputProfile::parse(sampleProfile(), profile, error));

    RawInputEvent raw;
    raw.pad_buttons = AURORA_BTN_A | AURORA_BTN_B;
    AuroraInputState state = applyProfile(profile, raw, 320, 240);
    CHECK((state.buttons & AURORA_BTN_A) != 0u);
    CHECK((state.buttons & AURORA_BTN_B) == 0u);

    raw.pad_buttons = 0;
    raw.key_code = 90;
    state = applyProfile(profile, raw, 320, 240);
    CHECK((state.buttons & AURORA_BTN_A) != 0u);
}

AURORA_TEST(input_maps_touch_zones_to_frame_pixels) {
    InputProfile profile;
    std::string error;
    CHECK(InputProfile::parse(sampleProfile(), profile, error));

    RawInputEvent raw;
    raw.touch_active = true;
    raw.touch_x = 0.5f;
    raw.touch_y = 0.95f;
    const AuroraInputState state = applyProfile(profile, raw, 320, 240);
    CHECK((state.buttons & AURORA_BTN_START) != 0u);
    CHECK_EQ(state.touch_active, static_cast<uint8_t>(1));
    CHECK_EQ(state.touch_x, 160);
    CHECK_EQ(state.touch_y, 228);

    raw.touch_x = 0.1f;
    raw.touch_y = 0.1f;
    const AuroraInputState outside = applyProfile(profile, raw, 320, 240);
    CHECK((outside.buttons & AURORA_BTN_START) == 0u);
}

AURORA_TEST(input_default_profile_covers_a_touch_screen) {
    const InputProfile profile = defaultProfile();
    CHECK(!profile.bindings.empty());
    bool has_touch = false;
    for (const auto& binding : profile.bindings) {
        if (binding.touch_w > 0.0f) has_touch = true;
    }
    CHECK(has_touch);
}
