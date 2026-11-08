// SPDX-License-Identifier: GPL-3.0-or-later
#include "aurora/library.h"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <filesystem>
#include <fstream>

namespace aurora {

namespace {

std::string lower(std::string text) {
    std::transform(text.begin(), text.end(), text.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return text;
}

std::string readMagic(const std::filesystem::path& path, std::size_t bytes) {
    std::ifstream stream(path, std::ios::binary);
    if (!stream.good()) return std::string();
    std::string buffer(bytes, '\0');
    stream.read(&buffer[0], static_cast<std::streamsize>(bytes));
    buffer.resize(static_cast<std::size_t>(stream.gcount()));
    std::string hex;
    char pair[3];
    for (unsigned char c : buffer) {
        std::snprintf(pair, sizeof(pair), "%02X", c);
        hex += pair;
    }
    return hex;
}

bool matchesMagic(const std::filesystem::path& file, const std::string& hex) {
    const std::string head = readMagic(file, std::max<std::size_t>(hex.size() / 2, 1));
    return !head.empty() && head.compare(0, hex.size(), hex) == 0;
}

} // namespace

std::string gameIdForPath(const std::string& path) {
    std::uint64_t hash = 1469598103934665603ull;
    for (unsigned char c : path) {
        hash ^= c;
        hash *= 1099511628211ull;
    }
    char text[17];
    std::snprintf(text, sizeof(text), "%016llx", static_cast<unsigned long long>(hash));
    return std::string(text);
}

GameEntry GameLibrary::classify(const std::string& path) const {
    GameEntry entry;
    std::error_code code;
    const std::filesystem::path target(path);
    if (!std::filesystem::exists(target, code)) return entry;

    std::filesystem::path candidate = target;
    bool is_directory = std::filesystem::is_directory(target, code);

    /* For a folder, look one level deep for the file that identifies the engine. */
    if (is_directory) {
        std::vector<std::filesystem::path> children;
        for (const auto& item : std::filesystem::directory_iterator(target, code)) {
            if (item.is_regular_file()) children.push_back(item.path());
        }
        std::sort(children.begin(), children.end());
        bool found = false;
        for (const auto& child : children) {
            const GameEntry probe = classify(child.generic_string());
            if (!probe.module_id.empty()) { candidate = child; found = true; break; }
        }
        if (!found) return entry;
    }

    const std::string name = lower(candidate.filename().generic_string());
    const std::string extension = lower(candidate.extension().generic_string());
    const std::string format = extension.empty() ? std::string() : extension.substr(1);

    for (const auto& module : registry_.entries()) {
        if (!module.manifest_valid) continue;
        const Manifest& manifest = module.manifest;

        for (const auto& hex : manifest.detect_magic) {
            if (matchesMagic(candidate, hex)) {
                entry.game_id = gameIdForPath(target.generic_string());
                entry.title = target.filename().generic_string();
                entry.path = target.generic_string();
                entry.module_id = manifest.module_id;
                entry.format = format;
                entry.detected_by = "magic:" + hex;
                return entry;
            }
        }
        for (const auto& expected : manifest.detect_filenames) {
            if (name == lower(expected)) {
                entry.game_id = gameIdForPath(target.generic_string());
                entry.title = target.filename().generic_string();
                entry.path = target.generic_string();
                entry.module_id = manifest.module_id;
                entry.format = format;
                entry.detected_by = "filename:" + expected;
                return entry;
            }
        }
        if (!format.empty()) {
            for (const auto& expected : manifest.detect_extensions) {
                if (format == lower(expected)) {
                    entry.game_id = gameIdForPath(target.generic_string());
                    entry.title = target.filename().generic_string();
                    entry.path = target.generic_string();
                    entry.module_id = manifest.module_id;
                    entry.format = format;
                    entry.detected_by = "extension:" + expected;
                    return entry;
                }
            }
        }
        /* A folder whose own name matched nothing but whose child did is handled
         * above; a folder that directly carries the module's extension is not. */
    }

    if (is_directory) return GameEntry();
    entry.game_id = gameIdForPath(target.generic_string());
    entry.title = target.filename().generic_string();
    entry.path = target.generic_string();
    entry.format = format;
    return entry;
}

std::vector<GameEntry> GameLibrary::scan(const std::string& root, int max_depth) const {
    std::vector<GameEntry> games;
    std::error_code code;
    const std::filesystem::path base(root);
    if (!std::filesystem::is_directory(base, code)) return games;

    /* Top-level items: a folder is one game (if it contains a recognisable file),
     * a loose file is one game. This keeps "game" looking like what a user means. */
    std::vector<std::filesystem::path> items;
    for (const auto& item : std::filesystem::directory_iterator(base, code)) {
        if (item.is_directory() || item.is_regular_file()) items.push_back(item.path());
    }
    std::sort(items.begin(), items.end());

    for (const auto& item : items) {
        const GameEntry entry = classify(item.generic_string());
        if (entry.game_id.empty()) continue;
        games.push_back(entry);
    }
    (void)max_depth; /* reserved: nested library layouts */
    std::sort(games.begin(), games.end(),
              [](const GameEntry& a, const GameEntry& b) { return a.title < b.title; });
    return games;
}

} // namespace aurora
