// SPDX-License-Identifier: GPL-3.0-or-later
#include <filesystem>
#include <fstream>
#include <string>

#include "aurora/library.h"
#include "test_env.h"
#include "test_harness.h"

using namespace aurora;

namespace {

std::string writeFile(const std::string& directory, const std::string& name,
                      const std::string& content) {
    std::filesystem::path path(directory);
    path /= name;
    std::ofstream stream(path, std::ios::binary);
    stream.write(content.data(), static_cast<std::streamsize>(content.size()));
    stream.close();
    return path.generic_string();
}

std::string xp3Manifest() {
    return R"json({
        "module_id": "com.aurora.runtime.kirikiri",
        "name": "KiriKiri (detection only)",
        "version": "1.0.0",
        "module_type": "galgame_engine",
        "platform": "krkr",
        "adapter": "visual_novel",
        "entry_symbol": "aurora_module_entry_kirikiri",
        "license": "GPL-3.0-or-later",
        "sdk_version": "1.0.0",
        "minimum_sdk_version": "1.0.0",
        "supported_formats": ["xp3"],
        "capabilities": {},
        "dependencies": [],
        "detection": { "extensions": ["xp3"], "magic": ["5850330A"] }
    })json";
}

std::string onsManifest() {
    return R"json({
        "module_id": "com.aurora.runtime.onscripter",
        "name": "ONScripter (detection only)",
        "version": "1.0.0",
        "module_type": "galgame_engine",
        "platform": "ons",
        "adapter": "visual_novel",
        "entry_symbol": "aurora_module_entry_onscripter",
        "license": "GPL-2.0-or-later",
        "sdk_version": "1.0.0",
        "minimum_sdk_version": "1.0.0",
        "supported_formats": ["dat"],
        "capabilities": {},
        "dependencies": [],
        "detection": { "filenames": ["nscript.dat"] }
    })json";
}

} // namespace

AURORA_TEST(library_matches_by_magic_before_extension) {
    const std::string directory = test::freshDirectory("library-magic");
    writeFile(directory, "first.xp3", std::string("XP3\n") + "payload");

    ModuleRegistry registry(test::sdkVersion());
    std::string error;
    CHECK(registry.addManifestJson(xp3Manifest(), "krkr.json", error));

    GameLibrary library(registry);
    const GameEntry entry = library.classify(directory + "/first.xp3");
    CHECK_EQ(entry.module_id, std::string("com.aurora.runtime.kirikiri"));
    CHECK_CONTAINS(entry.detected_by, "magic:");
    CHECK(!entry.game_id.empty());
}

AURORA_TEST(library_matches_by_entry_file_name) {
    const std::string directory = test::freshDirectory("library-filename");
    writeFile(directory, "nscript.dat", "some script");

    ModuleRegistry registry(test::sdkVersion());
    std::string error;
    CHECK(registry.addManifestJson(onsManifest(), "ons.json", error));

    GameLibrary library(registry);
    const GameEntry entry = library.classify(directory + "/nscript.dat");
    CHECK_EQ(entry.module_id, std::string("com.aurora.runtime.onscripter"));
    CHECK_CONTAINS(entry.detected_by, "filename:");
}

AURORA_TEST(library_scans_a_game_folder) {
    const std::string root = test::freshDirectory("library-folder");
    const std::string gameFolder = root + "/My Game";
    std::filesystem::create_directories(gameFolder);
    writeFile(gameFolder, "nscript.dat", "script");
    writeFile(gameFolder, "readme.txt", "hello");

    ModuleRegistry registry(test::sdkVersion());
    std::string error;
    CHECK(registry.addManifestJson(onsManifest(), "ons.json", error));

    GameLibrary library(registry);
    const std::vector<GameEntry> games = library.scan(root, 3);
    CHECK_EQ(games.size(), static_cast<std::size_t>(1));
    if (!games.empty()) {
        CHECK_EQ(games[0].module_id, std::string("com.aurora.runtime.onscripter"));
        CHECK_EQ(games[0].title, std::string("My Game"));
    }
}

AURORA_TEST(library_keeps_unrecognised_files_visible) {
    const std::string root = test::freshDirectory("library-unknown");
    writeFile(root, "mystery.bin", std::string("\x01\x02\x03", 3));

    ModuleRegistry registry(test::sdkVersion());
    std::string error;
    CHECK(registry.addManifestJson(xp3Manifest(), "krkr.json", error));

    GameLibrary library(registry);
    const std::vector<GameEntry> games = library.scan(root, 1);
    CHECK_EQ(games.size(), static_cast<std::size_t>(1));
    if (!games.empty()) {
        CHECK(games[0].module_id.empty());
        CHECK_EQ(games[0].format, std::string("bin"));
    }
}

AURORA_TEST(library_reports_a_game_whose_module_is_not_compiled) {
    const std::string directory = test::freshDirectory("library-not-compiled");
    writeFile(directory, "demo.iso", "no real magic");

    test::Environment env("library-env");
    GameLibrary library(env.registry);
    const GameEntry entry = library.classify(directory + "/demo.iso");
    CHECK_EQ(entry.module_id, std::string("com.aurora.runtime.ppsspp"));

    const ModuleEntry* module = env.registry.find(entry.module_id);
    CHECK(module != nullptr);
    if (module != nullptr) CHECK_EQ(module->state, AURORA_STATE_DISCOVERED);
}
