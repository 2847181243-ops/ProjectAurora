// SPDX-FileCopyrightText: 2026 Project Aurora contributors
// SPDX-License-Identifier: GPL-3.0-or-later

#include <string>

#include "aurora/manifest.h"
#include "test_harness.h"

using namespace aurora;

namespace {

void replaceAll(std::string& text, const std::string& from, const std::string& to) {
    std::size_t position = 0;
    while ((position = text.find(from, position)) != std::string::npos) {
        text.replace(position, from.size(), to);
        position += to.size();
    }
}

std::string validManifest() {
    return R"({
        "module_id": "com.aurora.runtime.ppsspp",
        "name": "PPSSPP",
        "version": "1.0.0",
        "module_type": "console_emulator",
        "platform": "psp",
        "adapter": "native",
        "entry_symbol": "aurora_module_entry_ppsspp",
        "license": "GPL-2.0-or-later",
        "sdk_version": "1.0.0",
        "minimum_sdk_version": "1.0.0",
        "supported_formats": ["iso", "cso", "chd", "pbp"],
        "capabilities": {
            "save_state": true,
            "controller": true,
            "touch_input": true,
            "fast_forward": true,
            "jit_optional": true,
            "video_output": true
        },
        "dependencies": [],
        "status": "ready"
    })";
}

bool hasError(const ManifestParseResult& result) {
    for (const auto& issue : result.issues) {
        if (issue.severity == "error") return true;
    }
    return false;
}

bool hasIssueOn(const ManifestParseResult& result, const std::string& field) {
    for (const auto& issue : result.issues) {
        if (issue.field == field) return true;
    }
    return false;
}

} // namespace

AURORA_TEST(manifest_accepts_a_complete_document) {
    const ManifestParseResult result = parseManifest(validManifest(), "inline");
    CHECK(result.ok);
    CHECK_EQ(result.manifest.module_id, std::string("com.aurora.runtime.ppsspp"));
    CHECK_EQ(result.manifest.kind, AURORA_KIND_CONSOLE_EMULATOR);
    CHECK_EQ(result.manifest.adapter, AdapterKind::Native);
    CHECK_EQ(result.manifest.supported_formats.size(), static_cast<std::size_t>(4));
    CHECK((result.manifest.capabilities & AURORA_CAP_SAVE_STATE) != 0u);
    CHECK((result.manifest.capabilities & AURORA_CAP_TOUCH) != 0u);
    CHECK(result.manifest.version_semver.valid);
}

AURORA_TEST(manifest_rejects_bad_module_id) {
    std::string text = validManifest();
    replaceAll(text, "com.aurora.runtime.ppsspp", "com.aurora..broken");
    const ManifestParseResult result = parseManifest(text, "inline");
    CHECK(!result.ok);
    CHECK(hasError(result));
    CHECK(hasIssueOn(result, "module_id"));
}

AURORA_TEST(manifest_rejects_incomplete_versions) {
    std::string text = validManifest();
    replaceAll(text, "\"version\": \"1.0.0\"", "\"version\": \"1.0\"");
    const ManifestParseResult result = parseManifest(text, "inline");
    CHECK(!result.ok);
    CHECK(hasIssueOn(result, "version"));
}

AURORA_TEST(manifest_rejects_minimum_newer_than_target) {
    std::string text = validManifest();
    replaceAll(text, "\"minimum_sdk_version\": \"1.0.0\"", "\"minimum_sdk_version\": \"2.0.0\"");
    const ManifestParseResult result = parseManifest(text, "inline");
    CHECK(!result.ok);
    CHECK(hasIssueOn(result, "minimum_sdk_version"));
}

AURORA_TEST(manifest_rejects_dotted_format) {
    std::string text = validManifest();
    replaceAll(text, "\"iso\"", "\".iso\"");
    const ManifestParseResult result = parseManifest(text, "inline");
    CHECK(!result.ok);
    CHECK(hasIssueOn(result, "supported_formats[0]"));
}

AURORA_TEST(manifest_rejects_duplicate_format) {
    std::string text = validManifest();
    replaceAll(text, "\"cso\"", "\"iso\"");
    const ManifestParseResult result = parseManifest(text, "inline");
    CHECK(!result.ok);
    bool duplicate = false;
    for (const auto& issue : result.issues) {
        if (issue.message.find("duplicate") != std::string::npos) duplicate = true;
    }
    CHECK(duplicate);
}

AURORA_TEST(manifest_rejects_self_dependency) {
    std::string text = validManifest();
    replaceAll(text, "\"dependencies\": []", "\"dependencies\": [\"com.aurora.runtime.ppsspp\"]");
    const ManifestParseResult result = parseManifest(text, "inline");
    CHECK(!result.ok);
}

AURORA_TEST(manifest_warns_but_accepts_unknown_capability) {
    std::string text = validManifest();
    replaceAll(text, "\"jit_optional\": true", "\"jit_optional\": true, \"teleportation\": true");
    const ManifestParseResult result = parseManifest(text, "inline");
    CHECK(result.ok);
    CHECK(hasIssueOn(result, "capabilities.teleportation"));
}

AURORA_TEST(manifest_rejects_missing_required_field) {
    std::string text = validManifest();
    replaceAll(text, "\"name\": \"PPSSPP\",", "");
    const ManifestParseResult result = parseManifest(text, "inline");
    CHECK(!result.ok);
    CHECK(hasIssueOn(result, "name"));
}
