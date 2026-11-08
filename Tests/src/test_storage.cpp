// SPDX-FileCopyrightText: 2026 Project Aurora contributors
// SPDX-License-Identifier: GPL-3.0-or-later

#include <filesystem>
#include <string>
#include <vector>

#include "aurora/storage.h"
#include "test_env.h"
#include "test_harness.h"

using namespace aurora;

AURORA_TEST(storage_accepts_only_safe_module_ids) {
    CHECK(StorageLayout::isSafeModuleId("com.aurora.runtime.ppsspp"));
    CHECK(StorageLayout::isSafeModuleId("com_aurora"));
    CHECK(!StorageLayout::isSafeModuleId("../escape"));
    CHECK(!StorageLayout::isSafeModuleId("com.aurora/evil"));
    CHECK(!StorageLayout::isSafeModuleId("Com.Aurora"));
    CHECK(!StorageLayout::isSafeModuleId(""));
}

AURORA_TEST(storage_rejects_relative_path_escapes) {
    const std::string root = test::freshDirectory("storage-escapes");
    StorageLayout layout(root);
    std::string error;

    CHECK(layout.resolve("com.aurora.runtime.simulated", AURORA_STORAGE_SAVES, "../evil.bin", error).empty());
    CHECK(!error.empty());
    CHECK(layout.resolve("com.aurora.runtime.simulated", AURORA_STORAGE_SAVES, "C:/windows", error).empty());
    CHECK(layout.resolve("com.aurora.runtime.simulated", AURORA_STORAGE_SAVES, "/etc/passwd", error).empty());
    CHECK(layout.resolve("com.aurora.runtime.simulated", AURORA_STORAGE_SAVES, "a/../../b", error).empty());
    CHECK(layout.resolve("../bad", AURORA_STORAGE_SAVES, "ok.bin", error).empty());

    const std::string inside =
        layout.resolve("com.aurora.runtime.simulated", AURORA_STORAGE_SAVES, "slot1.sav", error);
    CHECK(!inside.empty());
    CHECK(inside.find("RuntimeModules") != std::string::npos);
    CHECK(inside.find("slot1.sav") != std::string::npos);
}

AURORA_TEST(storage_keeps_modules_apart) {
    const std::string root = test::freshDirectory("storage-isolation");
    StorageLayout layout(root);
    std::string error;
    const std::string a = layout.resolve("com.aurora.runtime.simulated", AURORA_STORAGE_SAVES, "save.bin", error);
    const std::string b = layout.resolve("com.aurora.runtime.empty", AURORA_STORAGE_SAVES, "save.bin", error);
    CHECK(!a.empty());
    CHECK(!b.empty());
    CHECK(a != b);
    CHECK(a.find("com.aurora.runtime.simulated") != std::string::npos);
    CHECK(b.find("com.aurora.runtime.empty") != std::string::npos);
}

AURORA_TEST(storage_bucket_creation_is_idempotent) {
    const std::string root = test::freshDirectory("storage-buckets");
    StorageLayout layout(root);
    std::string error;
    CHECK(layout.ensureBucket("com.aurora.runtime.simulated", AURORA_STORAGE_STATES, error));
    CHECK(layout.ensureBucket("com.aurora.runtime.simulated", AURORA_STORAGE_STATES, error));
    const std::string directory = layout.bucketDirectory("com.aurora.runtime.simulated", AURORA_STORAGE_STATES);
    CHECK(std::filesystem::is_directory(directory));
}
