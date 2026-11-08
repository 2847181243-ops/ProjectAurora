// SPDX-FileCopyrightText: 2026 Project Aurora contributors
// SPDX-License-Identifier: GPL-3.0-or-later

/* Module manifest: declarative data, never code.
 *
 * A manifest says what a module *claims*. It never proves the module works. The
 * platform fuses three independent sources before it shows READY:
 *   1. this manifest (what was declared),
 *   2. the build record (what the toolchain actually compiled),
 *   3. the runtime probe (whether create_instance/initialize succeeded).
 */
#ifndef AURORA_MANIFEST_H
#define AURORA_MANIFEST_H

#include <string>
#include <vector>

#include "aurora/module_api.h"
#include "aurora/version.h"

namespace aurora {

enum class AdapterKind { Unknown = 0, Native, Libretro, VisualNovel, Web };
const char* adapterKindName(AdapterKind kind);
AdapterKind adapterKindFromName(const std::string& name);

struct Manifest {
    std::string module_id;
    std::string name;
    std::string version;
    std::string module_type;
    std::string platform;
    std::string sdk_version;
    std::string minimum_sdk_version;

    std::vector<std::string> supported_formats;
    AuroraCapabilityMask capabilities = AURORA_CAP_NONE;
    std::vector<std::string> capability_labels;
    std::vector<std::string> dependencies;

    /* Advisory only. Never used to decide runtime state. */
    std::string declared_status;

    AdapterKind adapter = AdapterKind::Unknown;
    std::string entry_symbol;
    std::string license;
    std::string upstream_project;
    std::string upstream_version;
    std::string upstream_commit;

    /* How the library recognises a game that belongs to this module. Rules are
     * data, not code: adding an engine must not mean editing the platform. */
    std::vector<std::string> detect_extensions;   /* "xp3", "iso" */
    std::vector<std::string> detect_filenames;    /* "nscript.dat", "start.ks" */
    std::vector<std::string> detect_magic;        /* "5850330A" hex, matched at offset 0 */

    AuroraRuntimeKind kind = AURORA_KIND_OTHER;
    SemVer version_semver;
    SemVer sdk_semver;
    SemVer min_sdk_semver;
};

struct ManifestIssue {
    std::string severity;  /* "error" or "warning" */
    std::string field;
    std::string message;
};

struct ManifestParseResult {
    bool ok = false;       /* true when no error-level issue was produced */
    Manifest manifest;
    std::vector<ManifestIssue> issues;
    std::string source;    /* file name or "inline" */
};

ManifestParseResult parseManifest(const std::string& json_text, const std::string& source);

bool isValidModuleId(const std::string& id);
bool isValidFormatToken(const std::string& token);
bool isValidPlatformToken(const std::string& token);

/* Returns AURORA_CAP_NONE and sets known=false for an unrecognised name.
 * Unknown names are a warning, not an error: an older host must be able to read a
 * manifest written for a newer SDK without dropping the whole module. */
AuroraCapabilityMask capabilityFromName(const std::string& name, bool& known);
std::vector<std::string> capabilityNames(AuroraCapabilityMask mask);

} // namespace aurora

#endif
