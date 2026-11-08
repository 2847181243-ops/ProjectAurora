#include <string>

#include "aurora/registry.h"
#include "test_env.h"
#include "test_harness.h"

using namespace aurora;

namespace {

const char* kDependentId = "com.aurora.test.dependent";
extern "C" const AuroraModuleVTable* aurora_module_entry_testdependent(void);

std::string dependentManifest(bool depend_on_empty) {
    std::string text = R"({
        "module_id": "com.aurora.test.dependent",
        "name": "Dependent Test Module",
        "version": "1.0.0",
        "module_type": "other",
        "platform": "test",
        "adapter": "native",
        "entry_symbol": "aurora_module_entry_testdependent",
        "license": "Apache-2.0",
        "sdk_version": "1.0.0",
        "minimum_sdk_version": "1.0.0",
        "supported_formats": ["bin"],
        "capabilities": {},
        "dependencies": [],
        "status": "ready"
    })";
    if (depend_on_empty) {
        const std::string from = "\"dependencies\": [],";
        const std::string to = "\"dependencies\": [\"com.aurora.runtime.empty\"],";
        const std::size_t position = text.find(from);
        if (position != std::string::npos) text.replace(position, from.size(), to);
    }
    return text;
}

} // namespace

AURORA_TEST(registry_derives_states_from_build_and_manifest) {
    test::Environment env("registry-basic");
    const ModuleEntry* simulated = env.registry.find("com.aurora.runtime.simulated");
    CHECK(simulated != nullptr);
    if (simulated != nullptr) CHECK_EQ(simulated->state, AURORA_STATE_READY);

    const ModuleEntry* ppsspp = env.registry.find("com.aurora.runtime.ppsspp");
    CHECK(ppsspp != nullptr);
    if (ppsspp != nullptr) {
        CHECK_EQ(ppsspp->state, AURORA_STATE_DISCOVERED);
        CHECK(!ppsspp->unmet.empty());
        if (!ppsspp->unmet.empty()) CHECK_CONTAINS(ppsspp->unmet.front(), "not compiled");
    }
}

AURORA_TEST(registry_ignores_a_claimed_ready_status) {
    test::Environment env("registry-claimed-ready");
    std::string text = test::shippedManifest("com.aurora.runtime.ppsspp.json");
    const std::string from = "\"status\": \"planned\"";
    const std::size_t position = text.find(from);
    CHECK(position != std::string::npos);
    if (position != std::string::npos) text.replace(position, from.size(), "\"status\": \"ready\"");

    ModuleRegistry registry(test::sdkVersion());
    std::string error;
    registry.addManifestJson(text, "claimed_ready.json", error);
    registry.evaluateAll();
    const ModuleEntry* entry = registry.find("com.aurora.runtime.ppsspp");
    CHECK(entry != nullptr);
    if (entry != nullptr) {
        CHECK_EQ(entry->manifest.declared_status, std::string("ready"));
        CHECK_EQ(entry->state, AURORA_STATE_DISCOVERED);
    }
}

AURORA_TEST(registry_disable_and_enable_keep_the_entry) {
    test::Environment env("registry-toggle");
    std::string error;
    CHECK(env.registry.setEnabled("com.aurora.runtime.empty", false, error));
    const ModuleEntry* entry = env.registry.find("com.aurora.runtime.empty");
    CHECK(entry != nullptr);
    if (entry != nullptr) CHECK_EQ(entry->state, AURORA_STATE_DISABLED);

    CHECK(env.registry.setEnabled("com.aurora.runtime.empty", true, error));
    entry = env.registry.find("com.aurora.runtime.empty");
    if (entry != nullptr) CHECK_EQ(entry->state, AURORA_STATE_READY);
}

AURORA_TEST(registry_blocks_a_module_whose_dependency_is_off) {
    test::Environment env("registry-dependency");
    std::string error;
    CHECK(env.registry.addManifestJson(dependentManifest(true), "dependent.json", error));
    CHECK(env.registry.attachBuiltin(&aurora_module_entry_testdependent, kDependentId, error));

    const ModuleEntry* dependent = env.registry.find(kDependentId);
    CHECK(dependent != nullptr);
    if (dependent != nullptr) CHECK_EQ(dependent->state, AURORA_STATE_READY);

    CHECK(env.registry.setEnabled("com.aurora.runtime.empty", false, error));
    dependent = env.registry.find(kDependentId);
    CHECK(dependent != nullptr);
    if (dependent != nullptr) {
        CHECK_EQ(dependent->state, AURORA_STATE_BUILT);
        bool blocked = false;
        for (const auto& reason : dependent->unmet) {
            if (reason.find("dependency_not_ready") != std::string::npos) blocked = true;
        }
        CHECK(blocked);
    }

    CHECK(env.registry.setEnabled("com.aurora.runtime.empty", true, error));
    dependent = env.registry.find(kDependentId);
    if (dependent != nullptr) CHECK_EQ(dependent->state, AURORA_STATE_READY);
}

AURORA_TEST(registry_rejects_duplicate_manifests) {
    test::Environment env("registry-duplicate");
    std::string error;
    const std::string text = test::shippedManifest("com.aurora.runtime.empty.json");
    CHECK(!env.registry.addManifestJson(text, "copy.json", error));
    CHECK_CONTAINS(error, "duplicate");
}

AURORA_TEST(registry_survives_an_invalid_manifest) {
    test::Environment env("registry-invalid");
    std::string error;
    CHECK(!env.registry.addManifestJson("{\"module_id\": \"com.aurora.broken\"}", "broken.json", error));
    const ModuleEntry* broken = env.registry.find("com.aurora.broken");
    CHECK(broken != nullptr);
    if (broken != nullptr) {
        CHECK_EQ(broken->state, AURORA_STATE_FAILED);
        CHECK_CONTAINS(broken->last_error, "manifest rejected");
    }
    /* The healthy modules are untouched by a neighbour that cannot even parse. */
    const ModuleEntry* simulated = env.registry.find("com.aurora.runtime.simulated");
    if (simulated != nullptr) CHECK_EQ(simulated->state, AURORA_STATE_READY);
}

AURORA_TEST(registry_refuses_an_sdk_mismatch) {
    test::Environment env("registry-sdk");
    std::string text = test::shippedManifest("com.aurora.runtime.simulated.json");
    const std::string from = "\"sdk_version\": \"1.0.0\"";
    const std::size_t position = text.find(from);
    CHECK(position != std::string::npos);
    if (position != std::string::npos) text.replace(position, from.size(), "\"sdk_version\": \"2.0.0\"");
    const std::size_t minimum = text.find("\"minimum_sdk_version\": \"1.0.0\"");
    if (minimum != std::string::npos) {
        text.replace(minimum, std::string("\"minimum_sdk_version\": \"1.0.0\"").size(),
                     "\"minimum_sdk_version\": \"2.0.0\"");
    }

    ModuleRegistry registry(test::sdkVersion());
    std::string error;
    CHECK(registry.addManifestJson(text, "future_sdk.json", error));
    for (std::size_t i = 0; i < aurora_builtin_module_count; ++i) {
        if (std::string(aurora_builtin_modules[i].module_id) == "com.aurora.runtime.simulated") {
            registry.attachBuiltin(aurora_builtin_modules[i].entry,
                                   aurora_builtin_modules[i].module_id, error);
        }
    }
    registry.evaluateAll();
    const ModuleEntry* entry = registry.find("com.aurora.runtime.simulated");
    CHECK(entry != nullptr);
    if (entry != nullptr) {
        CHECK_EQ(entry->state, AURORA_STATE_BUILT);
        bool sdk_reason = false;
        for (const auto& reason : entry->unmet) {
            if (reason.find("sdk:") != std::string::npos) sdk_reason = true;
        }
        CHECK(sdk_reason);
    }
}
