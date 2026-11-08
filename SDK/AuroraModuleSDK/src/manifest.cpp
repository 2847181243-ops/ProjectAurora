#include "aurora/manifest.h"

#include <algorithm>
#include <cctype>
#include <map>

#include "aurora/json.h"

namespace aurora {

namespace {

const std::map<std::string, AuroraCapabilityMask>& capabilityTable() {
    static const std::map<std::string, AuroraCapabilityMask> table = {
        {"save_state", AURORA_CAP_SAVE_STATE},
        {"game_saves", AURORA_CAP_GAME_SAVES},
        {"controller", AURORA_CAP_CONTROLLER},
        {"touch", AURORA_CAP_TOUCH},
        {"touch_input", AURORA_CAP_TOUCH},
        {"keyboard", AURORA_CAP_KEYBOARD},
        {"fast_forward", AURORA_CAP_FAST_FORWARD},
        {"audio", AURORA_CAP_AUDIO},
        {"video_output", AURORA_CAP_VIDEO_OUTPUT},
        {"jit_required", AURORA_CAP_JIT_REQUIRED},
        {"jit_optional", AURORA_CAP_JIT_OPTIONAL},
        {"shaders", AURORA_CAP_SHADERS},
        {"multiplayer", AURORA_CAP_MULTIPLAYER},
        {"requires_firmware", AURORA_CAP_REQUIRES_FIRMWARE},
    };
    return table;
}

void addIssue(std::vector<ManifestIssue>& issues, const char* severity,
              const std::string& field, const std::string& message) {
    issues.push_back({severity, field, message});
}

bool isLowerAlpha(char c) { return c >= 'a' && c <= 'z'; }
bool isDigit(char c) { return c >= '0' && c <= '9'; }

std::string requireString(const json::Value& root, const char* key,
                          std::vector<ManifestIssue>& issues, bool required) {
    const json::Value* value = root.find(key);
    if (value == nullptr) {
        if (required) addIssue(issues, "error", key, "missing required field");
        return std::string();
    }
    if (!value->isString() || value->string.empty()) {
        addIssue(issues, "error", key, std::string("must be a non-empty string, found ") + value->typeName());
        return std::string();
    }
    return value->string;
}

} // namespace

const char* adapterKindName(AdapterKind kind) {
    switch (kind) {
        case AdapterKind::Native: return "native";
        case AdapterKind::Libretro: return "libretro";
        case AdapterKind::VisualNovel: return "visual_novel";
        case AdapterKind::Web: return "web";
        case AdapterKind::Unknown: break;
    }
    return "unknown";
}

AdapterKind adapterKindFromName(const std::string& name) {
    if (name == "native") return AdapterKind::Native;
    if (name == "libretro") return AdapterKind::Libretro;
    if (name == "visual_novel") return AdapterKind::VisualNovel;
    if (name == "web") return AdapterKind::Web;
    return AdapterKind::Unknown;
}

bool isValidModuleId(const std::string& id) {
    if (id.size() < 3 || id.size() > 128) return false;
    if (!isLowerAlpha(id[0])) return false;
    bool segmentHasChar = true;
    bool lastWasDot = false;
    for (std::size_t i = 1; i < id.size(); ++i) {
        const char c = id[i];
        if (c == '.') {
            if (lastWasDot) return false;
            if (!segmentHasChar) return false;
            lastWasDot = true;
            segmentHasChar = false;
            continue;
        }
        if (!(isLowerAlpha(c) || isDigit(c) || c == '_')) return false;
        segmentHasChar = true;
        lastWasDot = false;
    }
    if (lastWasDot || !segmentHasChar) return false;
    return true;
}

bool isValidFormatToken(const std::string& token) {
    if (token.empty() || token.size() > 16) return false;
    for (char c : token) {
        if (!(isLowerAlpha(c) || isDigit(c) || c == '+' || c == '_' || c == '-')) return false;
    }
    return true;
}

bool isValidPlatformToken(const std::string& token) {
    return isValidFormatToken(token);
}

AuroraCapabilityMask capabilityFromName(const std::string& name, bool& known) {
    const auto& table = capabilityTable();
    auto it = table.find(name);
    if (it == table.end()) {
        known = false;
        return AURORA_CAP_NONE;
    }
    known = true;
    return it->second;
}

std::vector<std::string> capabilityNames(AuroraCapabilityMask mask) {
    std::vector<std::string> names;
    static const std::pair<const char*, AuroraCapabilityMask> order[] = {
        {"save_state", AURORA_CAP_SAVE_STATE},
        {"game_saves", AURORA_CAP_GAME_SAVES},
        {"controller", AURORA_CAP_CONTROLLER},
        {"touch_input", AURORA_CAP_TOUCH},
        {"keyboard", AURORA_CAP_KEYBOARD},
        {"fast_forward", AURORA_CAP_FAST_FORWARD},
        {"audio", AURORA_CAP_AUDIO},
        {"video_output", AURORA_CAP_VIDEO_OUTPUT},
        {"jit_required", AURORA_CAP_JIT_REQUIRED},
        {"jit_optional", AURORA_CAP_JIT_OPTIONAL},
        {"shaders", AURORA_CAP_SHADERS},
        {"multiplayer", AURORA_CAP_MULTIPLAYER},
        {"requires_firmware", AURORA_CAP_REQUIRES_FIRMWARE},
    };
    for (const auto& entry : order) {
        if (mask & entry.second) names.emplace_back(entry.first);
    }
    return names;
}

ManifestParseResult parseManifest(const std::string& json_text, const std::string& source) {
    ManifestParseResult result;
    result.source = source;

    const json::ParseResult parsed = json::parse(json_text);
    if (!parsed.ok) {
        addIssue(result.issues, "error", "<root>",
                 "invalid JSON at line " + std::to_string(parsed.error.line) +
                     ", column " + std::to_string(parsed.error.column) + ": " + parsed.error.message);
        return result;
    }
    if (!parsed.value.isObject()) {
        addIssue(result.issues, "error", "<root>", "manifest must be a JSON object");
        return result;
    }
    const json::Value& root = parsed.value;
    Manifest& m = result.manifest;

    m.module_id = requireString(root, "module_id", result.issues, true);
    m.name = requireString(root, "name", result.issues, true);
    m.version = requireString(root, "version", result.issues, true);
    m.module_type = requireString(root, "module_type", result.issues, true);
    m.platform = requireString(root, "platform", result.issues, true);
    m.sdk_version = requireString(root, "sdk_version", result.issues, true);
    m.minimum_sdk_version = requireString(root, "minimum_sdk_version", result.issues, true);
    m.adapter = adapterKindFromName(requireString(root, "adapter", result.issues, true));
    m.entry_symbol = requireString(root, "entry_symbol", result.issues, true);
    m.license = requireString(root, "license", result.issues, true);

    if (!m.module_id.empty() && !isValidModuleId(m.module_id)) {
        addIssue(result.issues, "error", "module_id",
                 "must be reverse-DNS, lowercase [a-z0-9_.], no empty segment: '" + m.module_id + "'");
    }
    if (!m.version.empty() && !SemVer::parse(m.version, m.version_semver)) {
        addIssue(result.issues, "error", "version", "not a MAJOR.MINOR.PATCH version: '" + m.version + "'");
    }
    if (!m.sdk_version.empty() && !SemVer::parse(m.sdk_version, m.sdk_semver)) {
        addIssue(result.issues, "error", "sdk_version", "not a MAJOR.MINOR.PATCH version: '" + m.sdk_version + "'");
    }
    if (!m.minimum_sdk_version.empty() && !SemVer::parse(m.minimum_sdk_version, m.min_sdk_semver)) {
        addIssue(result.issues, "error", "minimum_sdk_version",
                 "not a MAJOR.MINOR.PATCH version: '" + m.minimum_sdk_version + "'");
    }
    if (m.sdk_semver.valid && m.min_sdk_semver.valid &&
        m.min_sdk_semver.compare(m.sdk_semver) > 0) {
        addIssue(result.issues, "error", "minimum_sdk_version",
                 "minimum_sdk_version is newer than sdk_version");
    }
    if (!m.platform.empty() && !isValidPlatformToken(m.platform)) {
        addIssue(result.issues, "error", "platform", "must match [a-z0-9_+-]{1,16}: '" + m.platform + "'");
    }
    if (m.adapter == AdapterKind::Unknown) {
        addIssue(result.issues, "error", "adapter",
                 "must be one of native, libretro, visual_novel, web");
    }
    if (m.module_type == "console_emulator") {
        m.kind = AURORA_KIND_CONSOLE_EMULATOR;
    } else if (m.module_type == "galgame_engine") {
        m.kind = AURORA_KIND_GALGAME;
    } else if (m.module_type == "other") {
        m.kind = AURORA_KIND_OTHER;
    } else if (!m.module_type.empty()) {
        addIssue(result.issues, "error", "module_type",
                 "must be console_emulator, galgame_engine or other: '" + m.module_type + "'");
    }
    if (!m.entry_symbol.empty()) {
        const std::string& symbol = m.entry_symbol;
        const std::string prefix = "aurora_module_entry_";
        if (symbol.compare(0, prefix.size(), prefix) != 0) {
            addIssue(result.issues, "error", "entry_symbol",
                     "must start with aurora_module_entry_: '" + symbol + "'");
        }
    }

    const json::Value* formats = root.find("supported_formats");
    if (formats == nullptr) {
        addIssue(result.issues, "error", "supported_formats", "missing required field");
    } else if (!formats->isArray()) {
        addIssue(result.issues, "error", "supported_formats", "must be an array of strings");
    } else {
        for (std::size_t i = 0; i < formats->array.size(); ++i) {
            const json::Value& item = formats->array[i];
            const std::string field = "supported_formats[" + std::to_string(i) + "]";
            if (!item.isString()) {
                addIssue(result.issues, "error", field, "must be a string");
                continue;
            }
            if (!isValidFormatToken(item.string)) {
                addIssue(result.issues, "error", field,
                         "must match [a-z0-9_+-]{1,16} without a leading dot: '" + item.string + "'");
                continue;
            }
            if (std::find(m.supported_formats.begin(), m.supported_formats.end(), item.string) !=
                m.supported_formats.end()) {
                addIssue(result.issues, "error", field, "duplicate format '" + item.string + "'");
                continue;
            }
            m.supported_formats.push_back(item.string);
        }
        if (m.supported_formats.empty()) {
            addIssue(result.issues, "error", "supported_formats", "must list at least one format");
        }
    }

    const json::Value* capabilities = root.find("capabilities");
    if (capabilities != nullptr) {
        if (!capabilities->isObject()) {
            addIssue(result.issues, "error", "capabilities", "must be an object of booleans");
        } else {
            for (const auto& kv : capabilities->object) {
                const std::string field = "capabilities." + kv.first;
                if (!kv.second.isBool()) {
                    addIssue(result.issues, "error", field, "must be a boolean");
                    continue;
                }
                bool known = false;
                const AuroraCapabilityMask bit = capabilityFromName(kv.first, known);
                if (!known) {
                    addIssue(result.issues, "warning", field,
                             "unknown capability for this SDK version, ignored");
                    continue;
                }
                if (kv.second.boolean) {
                    m.capabilities |= bit;
                    m.capability_labels.push_back(kv.first);
                }
            }
        }
    }

    const json::Value* dependencies = root.find("dependencies");
    if (dependencies != nullptr) {
        if (!dependencies->isArray()) {
            addIssue(result.issues, "error", "dependencies", "must be an array of module ids");
        } else {
            for (std::size_t i = 0; i < dependencies->array.size(); ++i) {
                const json::Value& item = dependencies->array[i];
                const std::string field = "dependencies[" + std::to_string(i) + "]";
                if (!item.isString() || !isValidModuleId(item.string)) {
                    addIssue(result.issues, "error", field, "must be a valid module id");
                    continue;
                }
                if (item.string == m.module_id) {
                    addIssue(result.issues, "error", field, "a module cannot depend on itself");
                    continue;
                }
                if (std::find(m.dependencies.begin(), m.dependencies.end(), item.string) !=
                    m.dependencies.end()) {
                    addIssue(result.issues, "error", field, "duplicate dependency '" + item.string + "'");
                    continue;
                }
                m.dependencies.push_back(item.string);
            }
        }
    }

    const json::Value* status = root.find("status");
    if (status != nullptr) {
        if (!status->isString()) {
            addIssue(result.issues, "error", "status", "must be a string");
        } else {
            m.declared_status = status->string;
        }
    }

    const json::Value* upstream = root.find("upstream");
    if (upstream != nullptr) {
        if (!upstream->isObject()) {
            addIssue(result.issues, "error", "upstream", "must be an object");
        } else {
            const json::Value* project = upstream->find("project");
            const json::Value* version = upstream->find("version");
            const json::Value* commit = upstream->find("commit");
            if (project != nullptr && project->isString()) m.upstream_project = project->string;
            if (version != nullptr && version->isString()) m.upstream_version = version->string;
            if (commit != nullptr && commit->isString()) m.upstream_commit = commit->string;
        }
    }

    result.ok = true;
    for (const auto& issue : result.issues) {
        if (issue.severity == "error") {
            result.ok = false;
            break;
        }
    }
    return result;
}

} // namespace aurora
