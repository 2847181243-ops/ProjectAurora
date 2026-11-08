// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef AURORA_INPUT_PROFILE_H
#define AURORA_INPUT_PROFILE_H

#include <cstdint>
#include <string>
#include <vector>

#include "aurora/module_api.h"

namespace aurora {

/* Input mapping is data. A profile says which physical control drives which
 * logical button; the same file works for every module, so a user configures a
 * gamepad once instead of once per engine. */
enum class InputAction {
    A, B, X, Y, Up, Down, Left, Right, Start, Select, L1, R1
};

const char* inputActionName(InputAction action);
bool inputActionFromName(const std::string& name, InputAction& out);

struct InputBinding {
    InputAction action = InputAction::A;
    uint32_t pad_mask = 0;      /* AuroraButton bits */
    int32_t key_code = -1;      /* -1 = unbound */
    /* Normalised touch zone, 0..1 of the view. touch_w <= 0 means unbound. */
    float touch_x = 0.0f;
    float touch_y = 0.0f;
    float touch_w = 0.0f;
    float touch_h = 0.0f;
};

struct InputProfile {
    std::string name = "default";
    std::vector<InputBinding> bindings;

    static bool parse(const std::string& json, InputProfile& out, std::string& error);
};

/* Raw state coming from the OS, before mapping. Touch coordinates are normalised. */
struct RawInputEvent {
    uint32_t pad_buttons = 0;
    int16_t axis_left_x = 0;
    int16_t axis_left_y = 0;
    int32_t key_code = -1;
    bool touch_active = false;
    float touch_x = -1.0f;
    float touch_y = -1.0f;
};

/* Produces the snapshot a module sees. Touch zones land in frame pixels, which is
 * what the ABI promises modules. */
AuroraInputState applyProfile(const InputProfile& profile,
                              const RawInputEvent& raw,
                              uint32_t frame_width,
                              uint32_t frame_height);

/* The profile a module gets when the user has not configured one: sensible
 * defaults for a touch screen, no gamepad assumed. */
InputProfile defaultProfile();

} // namespace aurora

#endif
