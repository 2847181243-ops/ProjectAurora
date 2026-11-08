// SPDX-License-Identifier: GPL-3.0-or-later
#include "aurora/library.h"

#include <iterator>

#include "aurora/json.h"

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

namespace {

std::uint64_t fnv1a(const void* data, std::size_t size, std::uint64_t seed) {
    const unsigned char* bytes = static_cast<const unsigned char*>(data);
    std::uint64_t hash = seed;
    for (std::size_t i = 0; i < size; ++i) {
        hash ^= bytes[i];
        hash *= 1099511628211ull;
    }
    return hash;
}

} // namespace

std::string gameIdForContent(const std::string& entry_path) {
    std::uint64_t hash = 1469598103934665603ull;
    std::error_code code;
    const std::uintmax_t size = std::filesystem::exists(entry_path, code)
                                    ? std::filesystem::file_size(entry_path, code) : 0;
    const std::string size_text = std::to_string(static_cast<unsigned long long>(size));
    hash = fnv1a(size_text.data(), size_text.size(), hash);

    std::ifstream stream(entry_path, std::ios::binary);
    if (stream.good()) {
        std::string head(64 * 1024, '\0');
        stream.read(&head[0], static_cast<std::streamsize>(head.size()));
        head.resize(static_cast<std::size_t>(stream.gcount()));
        hash = fnv1a(head.data(), head.size(), hash);
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
                entry.game_id = gameIdForContent(candidate.generic_string());
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
                entry.game_id = gameIdForContent(candidate.generic_string());
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
                    entry.game_id = gameIdForContent(candidate.generic_string());
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
    entry.game_id = gameIdForContent(candidate.generic_string());
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


namespace {

std::string escapeJson(const std::string& text) {
    std::string out;
    out.reserve(text.size() + 8);
    for (char c : text) {
        switch (c) {
            case '"': out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default:
                if (static_cast<unsigned char>(c) < 0x20) {
                    char buffer[7];
                    std::snprintf(buffer, sizeof(buffer), "\\u%04x", c);
                    out += buffer;
                } else {
                    out += c;
                }
        }
    }
    return out;
}

} // namespace

bool saveGames(const std::string& json_path,
               const std::vector<GameEntry>& games,
               std::string& error) {
    error.clear();
    std::ofstream stream(json_path, std::ios::trunc);
    if (!stream.good()) {
        error = "cannot open library file for writing: " + json_path;
        return false;
    }
    stream << "[\n";
    for (std::size_t i = 0; i < games.size(); ++i) {
        const GameEntry& game = games[i];
        stream << "  {\n"
               << "    \"id\": \"" << escapeJson(game.game_id) << "\",\n"
               << "    \"title\": \"" << escapeJson(game.title) << "\",\n"
               << "    \"path\": \"" << escapeJson(game.path) << "\",\n"
               << "    \"module\": \"" << escapeJson(game.module_id) << "\",\n"
               << "    \"format\": \"" << escapeJson(game.format) << "\",\n"
               << "    \"detected_by\": \"" << escapeJson(game.detected_by) << "\"\n"
               << "  }" << (i + 1 == games.size() ? "" : ",") << "\n";
    }
    stream << "]\n";
    if (!stream.good()) {
        error = "failed while writing library file";
        return false;
    }
    return true;
}

bool loadGames(const std::string& json_path,
               std::vector<GameEntry>& games,
               std::string& error) {
    error.clear();
    games.clear();
    std::ifstream stream(json_path);
    if (!stream.good()) {
        error = "cannot open library file: " + json_path;
        return false;
    }
    const std::string text((std::istreambuf_iterator<char>(stream)),
                           std::istreambuf_iterator<char>());
    const json::ParseResult parsed = json::parse(text);
    if (!parsed.ok) {
        error = "invalid library JSON: " + parsed.error.message;
        return false;
    }
    if (!parsed.value.isArray()) {
        error = "library must be a JSON array";
        return false;
    }
    for (const auto& item : parsed.value.array) {
        if (!item.isObject()) continue;
        GameEntry game;
        if (const json::Value* value = item.find("id"); value && value->isString()) game.game_id = value->string;
        if (const json::Value* value = item.find("title"); value && value->isString()) game.title = value->string;
        if (const json::Value* value = item.find("path"); value && value->isString()) game.path = value->string;
        if (const json::Value* value = item.find("module"); value && value->isString()) game.module_id = value->string;
        if (const json::Value* value = item.find("format"); value && value->isString()) game.format = value->string;
        if (const json::Value* value = item.find("detected_by"); value && value->isString()) game.detected_by = value->string;
        if (!game.path.empty()) games.push_back(std::move(game));
    }
    return true;
}

} // namespace aurora
