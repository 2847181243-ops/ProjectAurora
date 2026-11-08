// SPDX-License-Identifier: GPL-3.0-or-later
#include "aurora/input_profile.h"

#include <cstring>
#include <map>

#include "aurora/json.h"

namespace aurora {

namespace {

const std::map<std::string, InputAction>& actionTable() {
    static const std::map<std::string, InputAction> table = {
        {"a", InputAction::A}, {"b", InputAction::B}, {"x", InputAction::X},
        {"y", InputAction::Y}, {"up", InputAction::Up}, {"down", InputAction::Down},
        {"left", InputAction::Left}, {"right", InputAction::Right},
        {"start", InputAction::Start}, {"select", InputAction::Select},
        {"l1", InputAction::L1}, {"r1", InputAction::R1},
    };
    return table;
}

uint32_t maskFor(InputAction action) {
    switch (action) {
        case InputAction::A: return AURORA_BTN_A;
        case InputAction::B: return AURORA_BTN_B;
        case InputAction::X: return AURORA_BTN_X;
        case InputAction::Y: return AURORA_BTN_Y;
        case InputAction::Up: return AURORA_BTN_UP;
        case InputAction::Down: return AURORA_BTN_DOWN;
        case InputAction::Left: return AURORA_BTN_LEFT;
        case InputAction::Right: return AURORA_BTN_RIGHT;
        case InputAction::Start: return AURORA_BTN_START;
        case InputAction::Select: return AURORA_BTN_SELECT;
        case InputAction::L1: return AURORA_BTN_L1;
        case InputAction::R1: return AURORA_BTN_R1;
    }
    return 0u;
}

float numberOr(const json::Value* object, const char* key, float fallback) {
    if (object == nullptr) return fallback;
    const json::Value* value = object->find(key);
    if (value == nullptr || !value->isNumber()) return fallback;
    return static_cast<float>(value->number);
}

} // namespace

const char* inputActionName(InputAction action) {
    switch (action) {
        case InputAction::A: return "a";
        case InputAction::B: return "b";
        case InputAction::X: return "x";
        case InputAction::Y: return "y";
        case InputAction::Up: return "up";
        case InputAction::Down: return "down";
        case InputAction::Left: return "left";
        case InputAction::Right: return "right";
        case InputAction::Start: return "start";
        case InputAction::Select: return "select";
        case InputAction::L1: return "l1";
        case InputAction::R1: return "r1";
    }
    return "?";
}

bool inputActionFromName(const std::string& name, InputAction& out) {
    const auto& table = actionTable();
    const auto it = table.find(name);
    if (it == table.end()) return false;
    out = it->second;
    return true;
}

bool InputProfile::parse(const std::string& json_text, InputProfile& out, std::string& error) {
    error.clear();
    const json::ParseResult parsed = json::parse(json_text);
    if (!parsed.ok) {
        error = "invalid JSON: " + parsed.error.message;
        return false;
    }
    if (!parsed.value.isObject()) {
        error = "profile must be an object";
        return false;
    }
    InputProfile profile;
    const json::Value* name = parsed.value.find("name");
    if (name != nullptr && name->isString()) profile.name = name->string;

    const json::Value* bindings = parsed.value.find("bindings");
    if (bindings == nullptr || !bindings->isArray()) {
        error = "bindings must be an array";
        return false;
    }
    for (const auto& item : bindings->array) {
        if (!item.isObject()) {
            error = "each binding must be an object";
            return false;
        }
        const json::Value* action = item.find("action");
        if (action == nullptr || !action->isString()) {
            error = "binding needs an action name";
            return false;
        }
        InputBinding binding;
        if (!inputActionFromName(action->string, binding.action)) {
            error = "unknown action: " + action->string;
            return false;
        }
        const json::Value* pad = item.find("pad_mask");
        if (pad != nullptr && pad->isNumber()) {
            binding.pad_mask = static_cast<uint32_t>(pad->number);
        }
        const json::Value* key = item.find("key_code");
        if (key != nullptr && key->isNumber()) {
            binding.key_code = static_cast<int32_t>(key->number);
        }
        const json::Value* touch = item.find("touch");
        if (touch != nullptr && touch->isObject()) {
            binding.touch_x = numberOr(touch, "x", 0.0f);
            binding.touch_y = numberOr(touch, "y", 0.0f);
            binding.touch_w = numberOr(touch, "w", 0.0f);
            binding.touch_h = numberOr(touch, "h", 0.0f);
        }
        profile.bindings.push_back(binding);
    }
    out = std::move(profile);
    return true;
}

AuroraInputState applyProfile(const InputProfile& profile,
                              const RawInputEvent& raw,
                              uint32_t frame_width,
                              uint32_t frame_height) {
    AuroraInputState state{};
    state.axis_left_x = raw.axis_left_x;
    state.axis_left_y = raw.axis_left_y;

    for (const auto& binding : profile.bindings) {
        if (binding.pad_mask != 0u && (raw.pad_buttons & binding.pad_mask) != 0u) {
            state.buttons |= maskFor(binding.action);
            continue;
        }
        if (binding.key_code >= 0 && raw.key_code >= 0 && raw.key_code == binding.key_code) {
            state.buttons |= maskFor(binding.action);
            continue;
        }
        if (binding.touch_w > 0.0f && binding.touch_h > 0.0f && raw.touch_active) {
            const bool inside = raw.touch_x >= binding.touch_x &&
                                raw.touch_x <= binding.touch_x + binding.touch_w &&
                                raw.touch_y >= binding.touch_y &&
                                raw.touch_y <= binding.touch_y + binding.touch_h;
            if (inside) state.buttons |= maskFor(binding.action);
        }
    }

    if (raw.touch_active && raw.touch_x >= 0.0f && raw.touch_y >= 0.0f) {
        state.touch_active = 1;
        /* Modules get frame pixels, not normalised coordinates. */
        state.touch_x = static_cast<int32_t>(raw.touch_x * static_cast<float>(frame_width));
        state.touch_y = static_cast<int32_t>(raw.touch_y * static_cast<float>(frame_height));
    }
    return state;
}

InputProfile defaultProfile() {
    InputProfile profile;
    profile.name = "default";
    /* Bottom-right cluster plus a d-pad cross, sized for thumb reach on a phone. */
    const struct { InputAction action; float x, y, w, h; } zones[] = {
        {InputAction::A, 0.76f, 0.70f, 0.12f, 0.16f},
        {InputAction::B, 0.88f, 0.58f, 0.12f, 0.16f},
        {InputAction::Start, 0.44f, 0.90f, 0.12f, 0.08f},
        {InputAction::Up, 0.06f, 0.42f, 0.10f, 0.10f},
        {InputAction::Down, 0.06f, 0.66f, 0.10f, 0.10f},
        {InputAction::Left, 0.00f, 0.54f, 0.10f, 0.10f},
        {InputAction::Right, 0.12f, 0.54f, 0.10f, 0.10f},
    };
    for (const auto& zone : zones) {
        InputBinding binding;
        binding.action = zone.action;
        binding.touch_x = zone.x;
        binding.touch_y = zone.y;
        binding.touch_w = zone.w;
        binding.touch_h = zone.h;
        profile.bindings.push_back(binding);
    }
    return profile;
}

} // namespace aurora
