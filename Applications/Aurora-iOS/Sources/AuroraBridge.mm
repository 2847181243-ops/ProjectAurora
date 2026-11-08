// SPDX-FileCopyrightText: 2026 Project Aurora contributors
// SPDX-License-Identifier: GPL-3.0-or-later

#import <Foundation/Foundation.h>

#include <algorithm>
#include <cstring>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "aurora/builtin_modules.h"
#include "aurora/diagnostics.h"
#include "aurora/host.h"
#include "aurora/display.h"
#include "aurora/input_profile.h"
#include "aurora/library.h"
#include "aurora/registry.h"
#include "aurora/supervisor.h"

#include "AuroraBridge.h"

namespace {

struct CachedRow {
    std::string module_id;
    std::string name;
    std::string platform;
    std::string detail;
    int32_t state = 0;
    int32_t enabled = 1;
    int32_t compiled = 0;
    int32_t frames = 0;
};

struct BridgeState {
    std::string storage_root;
    std::unique_ptr<aurora::Diagnostics> diagnostics;
    std::unique_ptr<aurora::ModuleHost> host;
    std::unique_ptr<aurora::ModuleRegistry> registry;
    std::unique_ptr<aurora::ModuleSupervisor> supervisor;
    std::string running_module;
    std::string last_error;
    std::string games_root;
    std::vector<aurora::GameEntry> games;
    std::string game_path = "/library/demo.sim";
    std::string game_format = "sim";
    std::vector<CachedRow> rows;
};

BridgeState& state() {
    static BridgeState instance;
    return instance;
}

std::string joinedDetail(const aurora::ModuleEntry& entry) {
    if (!entry.last_error.empty()) return entry.last_error;
    std::string text;
    for (std::size_t i = 0; i < entry.unmet.size(); ++i) {
        if (i != 0) text += ", ";
        text += entry.unmet[i];
    }
    return text;
}

const CachedRow* rowAt(int32_t index) {
    BridgeState& s = state();
    if (!s.registry) return nullptr;
    const auto& entries = s.registry->entries();
    if (index < 0 || static_cast<std::size_t>(index) >= entries.size()) return nullptr;

    s.rows.resize(entries.size());
    for (std::size_t i = 0; i < entries.size(); ++i) {
        const aurora::ModuleEntry& entry = entries[i];
        CachedRow& row = s.rows[i];
        row.module_id = entry.manifest.module_id;
        row.name = entry.manifest.name;
        row.platform = entry.manifest.platform;
        row.detail = joinedDetail(entry);
        row.state = static_cast<int32_t>(entry.state);
        row.enabled = entry.user_enabled ? 1 : 0;
        row.compiled = entry.compiled_in ? 1 : 0;
        row.frames = 0;
        if (s.supervisor) {
            if (const aurora::RuntimeInstance* instance = s.supervisor->instance(entry.manifest.module_id)) {
                row.frames = static_cast<int32_t>(instance->status.frames_presented);
            }
        }
    }
    return &s.rows[static_cast<std::size_t>(index)];
}

/* Manifests are a bundle resource. Depending on how the folder reference is
 * copied they land either in a ModuleManifests directory or straight in the bundle
 * root, so both layouts are accepted. */
std::vector<std::string> bundleManifestDirectories(const char* manifest_directory) {
    std::vector<std::string> directories;
    if (manifest_directory != nullptr && manifest_directory[0] != '\0') {
        directories.emplace_back(manifest_directory);
        return directories;
    }
    NSString* nested = [[NSBundle mainBundle] pathForResource:@"ModuleManifests" ofType:nil];
    if (nested != nil) directories.emplace_back([nested UTF8String]);
    NSString* root = [[NSBundle mainBundle] bundlePath];
    if (root != nil) directories.emplace_back([root UTF8String]);
    return directories;
}

int loadManifestsFrom(aurora::ModuleRegistry& registry, const std::string& directory) {
    std::error_code code;
    if (directory.empty() || !std::filesystem::is_directory(directory, code)) return 0;
    std::vector<std::filesystem::path> paths;
    for (const auto& item : std::filesystem::directory_iterator(directory, code)) {
        if (item.is_regular_file() && item.path().extension() == ".json") {
            paths.push_back(item.path());
        }
    }
    std::sort(paths.begin(), paths.end());
    int loaded = 0;
    for (const auto& item : paths) {
        NSData* data = [NSData dataWithContentsOfFile:[NSString stringWithUTF8String:item.generic_string().c_str()]];
        if (data == nil) continue;
        const std::string text(static_cast<const char*>(data.bytes), data.length);
        std::string error;
        if (registry.addManifestJson(text, item.filename().generic_string(), error)) {
            ++loaded;
        } else if (error.find("duplicate") == std::string::npos) {
            /* A stray JSON that is not a manifest is ignored on purpose: the bundle
             * root is scanned as a fallback and may hold unrelated files. */
        }
    }
    return loaded;
}

} // namespace

extern "C" {

int32_t aurora_bridge_init(const char* storage_root, const char* manifest_directory) {
    /* Tear the previous stack down first: assigning a new registry under a live
     * supervisor leaves it holding a reference to freed memory. */
    aurora_bridge_shutdown();

    BridgeState& s = state();
    s.storage_root = storage_root != nullptr ? storage_root : "";
    s.last_error.clear();

    {
        std::error_code code;
        std::filesystem::path games = std::filesystem::path(s.storage_root);
        games /= "Games";
        std::filesystem::create_directories(games, code);
        s.games_root = games.generic_string();
    }

    aurora::SemVer sdk;
    aurora::SemVer::parse("1.1.0", sdk);

    s.diagnostics = std::make_unique<aurora::Diagnostics>();
    s.diagnostics->setClockSource(&aurora::ModuleHost::monotonicSeconds);
    s.host = std::make_unique<aurora::ModuleHost>(s.storage_root, *s.diagnostics);
    s.host->setDefaultSurface(480, 320);
    s.registry = std::make_unique<aurora::ModuleRegistry>(sdk);
    s.registry->setFirmwareProbe([](const std::string&) { return false; });

    int manifest_count = 0;
    for (const std::string& directory : bundleManifestDirectories(manifest_directory)) {
        manifest_count += loadManifestsFrom(*s.registry, directory);
        if (manifest_count > 0) break;
    }
    if (manifest_count == 0) {
        s.last_error = "no module manifests found in the app bundle";
    }
    for (std::size_t i = 0; i < aurora_builtin_module_count; ++i) {
        std::string error;
        s.registry->attachBuiltin(aurora_builtin_modules[i].entry,
                                  aurora_builtin_modules[i].module_id, error);
    }
    s.registry->evaluateAll();
    s.supervisor = std::make_unique<aurora::ModuleSupervisor>(*s.registry, *s.host);
    return static_cast<int32_t>(s.registry->entries().size());
}

void aurora_bridge_shutdown(void) {
    BridgeState& s = state();
    if (s.supervisor) s.supervisor->shutdownAll();
    s.supervisor.reset();
    s.registry.reset();
    s.host.reset();
    s.diagnostics.reset();
    s.running_module.clear();
    s.rows.clear();
}

int32_t aurora_bridge_module_count(void) {
    BridgeState& s = state();
    return s.registry ? static_cast<int32_t>(s.registry->entries().size()) : 0;
}

const char* aurora_bridge_module_id(int32_t index) {
    const CachedRow* row = rowAt(index);
    return row != nullptr ? row->module_id.c_str() : "";
}

const char* aurora_bridge_module_name(int32_t index) {
    const CachedRow* row = rowAt(index);
    return row != nullptr ? row->name.c_str() : "";
}

const char* aurora_bridge_module_platform(int32_t index) {
    const CachedRow* row = rowAt(index);
    return row != nullptr ? row->platform.c_str() : "";
}

const char* aurora_bridge_module_detail(int32_t index) {
    const CachedRow* row = rowAt(index);
    return row != nullptr ? row->detail.c_str() : "";
}

int32_t aurora_bridge_module_state(int32_t index) {
    const CachedRow* row = rowAt(index);
    return row != nullptr ? row->state : -1;
}

int32_t aurora_bridge_module_enabled(int32_t index) {
    const CachedRow* row = rowAt(index);
    return row != nullptr ? row->enabled : 0;
}

int32_t aurora_bridge_module_compiled(int32_t index) {
    const CachedRow* row = rowAt(index);
    return row != nullptr ? row->compiled : 0;
}

int32_t aurora_bridge_module_frames(int32_t index) {
    const CachedRow* row = rowAt(index);
    return row != nullptr ? row->frames : 0;
}

int32_t aurora_bridge_set_enabled(const char* module_id, int32_t enabled) {
    BridgeState& s = state();
    if (!s.registry || module_id == nullptr) return AURORA_E_INVALID_ARGUMENT;
    std::string error;
    if (!s.registry->setEnabled(module_id, enabled != 0, error)) {
        s.last_error = error;
        return AURORA_E_STATE;
    }
    s.rows.clear();
    return AURORA_OK;
}

int32_t aurora_bridge_start(const char* module_id, const char* game_path, const char* format) {
    BridgeState& s = state();
    if (!s.supervisor || module_id == nullptr) return AURORA_E_INVALID_ARGUMENT;
    if (!s.running_module.empty() && s.running_module != module_id) {
        std::string ignored;
        s.supervisor->stop(s.running_module, ignored);
        s.running_module.clear();
    }
    if (game_path != nullptr && game_path[0] != '\0') s.game_path = game_path;
    if (format != nullptr && format[0] != '\0') s.game_format = format;

    AuroraGameInfo game{};
    game.game_path = s.game_path.c_str();
    game.format = s.game_format.c_str();
    game.display_name = "Demo";

    std::string error;
    const AuroraStatus status = s.supervisor->start(module_id, game, error);
    if (status != AURORA_OK) {
        s.last_error = error;
        return status;
    }
    s.running_module = module_id;
    s.last_error.clear();
    s.rows.clear();
    return AURORA_OK;
}

void aurora_bridge_tick(double delta_seconds) {
    BridgeState& s = state();
    if (!s.supervisor || s.running_module.empty()) return;
    std::string error;
    s.supervisor->tick(s.running_module, delta_seconds, error);
    if (!error.empty()) s.last_error = error;
}

int32_t aurora_bridge_stop(const char* module_id) {
    BridgeState& s = state();
    if (!s.supervisor || module_id == nullptr) return AURORA_E_INVALID_ARGUMENT;
    std::string error;
    const AuroraStatus status = s.supervisor->stop(module_id, error);
    if (s.running_module == module_id) s.running_module.clear();
    if (status != AURORA_OK) s.last_error = error;
    s.rows.clear();
    return status;
}

int32_t aurora_bridge_reset_failure(const char* module_id) {
    BridgeState& s = state();
    if (!s.supervisor || module_id == nullptr) return AURORA_E_INVALID_ARGUMENT;
    std::string error;
    const bool ok = s.supervisor->resetFailure(module_id, error);
    if (!ok) s.last_error = error;
    s.rows.clear();
    return ok ? AURORA_OK : AURORA_E_STATE;
}

const uint8_t* aurora_bridge_frame_data(void) {
    BridgeState& s = state();
    if (!s.supervisor || s.running_module.empty()) return nullptr;
    const aurora::RuntimeInstance* instance = s.supervisor->instance(s.running_module);
    if (instance == nullptr || instance->binding == nullptr) return nullptr;
    if (!instance->binding->surface_enabled || instance->binding->surface_pixels.empty()) return nullptr;
    return instance->binding->surface_pixels.data();
}

int32_t aurora_bridge_frame_width(void) {
    BridgeState& s = state();
    if (!s.supervisor || s.running_module.empty()) return 0;
    const aurora::RuntimeInstance* instance = s.supervisor->instance(s.running_module);
    return instance != nullptr && instance->binding != nullptr
               ? static_cast<int32_t>(instance->binding->surface_width) : 0;
}

int32_t aurora_bridge_frame_height(void) {
    BridgeState& s = state();
    if (!s.supervisor || s.running_module.empty()) return 0;
    const aurora::RuntimeInstance* instance = s.supervisor->instance(s.running_module);
    return instance != nullptr && instance->binding != nullptr
               ? static_cast<int32_t>(instance->binding->surface_height) : 0;
}

int32_t aurora_bridge_frames_presented(void) {
    BridgeState& s = state();
    if (!s.supervisor || s.running_module.empty()) return 0;
    const aurora::RuntimeInstance* instance = s.supervisor->instance(s.running_module);
    return instance != nullptr ? static_cast<int32_t>(instance->status.frames_presented) : 0;
}

const char* aurora_bridge_running_module(void) {
    return state().running_module.c_str();
}

const char* aurora_bridge_last_error(void) {
    return state().last_error.c_str();
}

const char* aurora_bridge_state_name(int32_t state_value) {
    return aurora_module_state_string(static_cast<AuroraModuleState>(state_value));
}


/* ---- library -------------------------------------------------------------- */

const char* aurora_bridge_games_root(void) {
    return state().games_root.c_str();
}

int32_t aurora_bridge_library_scan(void) {
    BridgeState& s = state();
    if (!s.registry) return 0;
    aurora::GameLibrary library(*s.registry);
    s.games = library.scan(s.games_root, 2);
    return static_cast<int32_t>(s.games.size());
}

int32_t aurora_bridge_library_count(void) {
    return static_cast<int32_t>(state().games.size());
}

static const aurora::GameEntry* gameAt(int32_t index) {
    BridgeState& s = state();
    if (index < 0 || static_cast<std::size_t>(index) >= s.games.size()) return nullptr;
    return &s.games[static_cast<std::size_t>(index)];
}

const char* aurora_bridge_library_title(int32_t index) {
    const aurora::GameEntry* game = gameAt(index);
    return game != nullptr ? game->title.c_str() : "";
}

const char* aurora_bridge_library_module(int32_t index) {
    const aurora::GameEntry* game = gameAt(index);
    return game != nullptr ? game->module_id.c_str() : "";
}

const char* aurora_bridge_library_state(int32_t index) {
    BridgeState& s = state();
    const aurora::GameEntry* game = gameAt(index);
    if (game == nullptr || game->module_id.empty() || !s.registry) return "UNMATCHED";
    const aurora::ModuleEntry* module = s.registry->find(game->module_id);
    if (module == nullptr) return "UNKNOWN";
    return aurora_module_state_string(module->state);
}

const char* aurora_bridge_library_detail(int32_t index) {
    static std::string detail;
    BridgeState& s = state();
    const aurora::GameEntry* game = gameAt(index);
    if (game == nullptr) return "";
    detail = game->detected_by;
    if (game->module_id.empty()) {
        detail += " (no module claims this)";
        return detail.c_str();
    }
    const aurora::ModuleEntry* module = s.registry ? s.registry->find(game->module_id) : nullptr;
    if (module == nullptr) return detail.c_str();
    if (!module->last_error.empty()) detail += " - " + module->last_error;
    else if (!module->unmet.empty()) {
        for (const auto& reason : module->unmet) detail += " - " + reason;
    }
    return detail.c_str();
}

int32_t aurora_bridge_library_import(const char* source_path) {
    BridgeState& s = state();
    if (source_path == nullptr || s.games_root.empty()) return -1;
    std::error_code code;
    const std::filesystem::path source(source_path);
    if (!std::filesystem::exists(source, code)) return -1;
    std::filesystem::path target(std::filesystem::path(s.games_root) / source.filename());
    std::filesystem::copy(source, target,
                          std::filesystem::copy_options::recursive |
                              std::filesystem::copy_options::overwrite_existing,
                          code);
    if (code) {
        s.last_error = "import failed: " + code.message();
        return -1;
    }
    return 0;
}

/* ---- input ---------------------------------------------------------------- */

void aurora_bridge_set_touch(int32_t active, float x, float y) {
    BridgeState& s = state();
    if (!s.supervisor || !s.host || s.running_module.empty()) return;
    const aurora::RuntimeInstance* instance = s.supervisor->instance(s.running_module);
    if (instance == nullptr || instance->binding == nullptr) return;

    aurora::RawInputEvent raw;
    raw.touch_active = active != 0;
    raw.touch_x = x;
    raw.touch_y = y;

    const aurora::InputProfile profile = aurora::defaultProfile();
    const AuroraInputState snapshot = aurora::applyProfile(
        profile, raw,
        instance->binding->surface_width,
        instance->binding->surface_height);
    s.host->setInput(*instance->binding, snapshot);
}

/* ---- display -------------------------------------------------------------- */

void aurora_bridge_compute_viewport(int32_t frame_width, int32_t frame_height,
                                    int32_t view_width, int32_t view_height,
                                    int32_t mode,
                                    int32_t* out_x, int32_t* out_y,
                                    int32_t* out_width, int32_t* out_height) {
    aurora::DisplaySettings settings;
    switch (mode) {
        case 1: settings.mode = aurora::ScaleMode::Fill; break;
        case 2: settings.mode = aurora::ScaleMode::Integer; break;
        case 3: settings.mode = aurora::ScaleMode::Stretch; break;
        default: settings.mode = aurora::ScaleMode::Fit; break;
    }
    const aurora::Viewport viewport = aurora::computeViewport(
        frame_width, frame_height, view_width, view_height, settings);
    if (out_x) *out_x = viewport.x;
    if (out_y) *out_y = viewport.y;
    if (out_width) *out_width = viewport.width;
    if (out_height) *out_height = viewport.height;
}

} // extern "C"
