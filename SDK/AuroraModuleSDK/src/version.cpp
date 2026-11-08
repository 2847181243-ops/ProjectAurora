// SPDX-FileCopyrightText: 2026 Project Aurora contributors
// SPDX-License-Identifier: GPL-3.0-or-later

#include "aurora/version.h"

#include <cstdio>

namespace aurora {

namespace {
bool parseNumber(const std::string& text, std::size_t& index, int& out) {
    std::size_t start = index;
    long value = 0;
    while (index < text.size() && text[index] >= '0' && text[index] <= '9') {
        value = value * 10 + (text[index] - '0');
        if (value > 1000000) return false;
        ++index;
    }
    if (index == start) return false;
    out = static_cast<int>(value);
    return true;
}
} // namespace

bool SemVer::parse(const std::string& text, SemVer& out) {
    out = SemVer();
    std::size_t index = 0;
    if (!parseNumber(text, index, out.major)) return false;
    if (index >= text.size() || text[index] != '.') return false;
    ++index;
    if (!parseNumber(text, index, out.minor)) return false;
    if (index >= text.size() || text[index] != '.') return false;
    ++index;
    if (!parseNumber(text, index, out.patch)) return false;
    if (index < text.size()) {
        if (text[index] == '-') {
            ++index;
            std::size_t start = index;
            while (index < text.size() && text[index] != '+') ++index;
            if (index == start) return false;
            out.prerelease = text.substr(start, index - start);
        }
        if (index < text.size() && text[index] == '+') {
            ++index;
            std::size_t start = index;
            while (index < text.size()) ++index;
            if (index == start) return false;
        }
        if (index < text.size()) return false;
    }
    out.valid = true;
    return true;
}

int SemVer::compare(const SemVer& other) const {
    if (major != other.major) return major < other.major ? -1 : 1;
    if (minor != other.minor) return minor < other.minor ? -1 : 1;
    if (patch != other.patch) return patch < other.patch ? -1 : 1;
    if (prerelease.empty() && other.prerelease.empty()) return 0;
    if (prerelease.empty()) return 1;   /* 1.0.0 > 1.0.0-rc1 */
    if (other.prerelease.empty()) return -1;
    if (prerelease == other.prerelease) return 0;
    return prerelease < other.prerelease ? -1 : 1;
}

std::string SemVer::toString() const {
    std::string text = std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(patch);
    if (!prerelease.empty()) text += "-" + prerelease;
    return text;
}

Compatibility checkSdkCompatibility(const SemVer& moduleBuiltAgainst,
                                    const SemVer& moduleMinimum,
                                    const SemVer& host) {
    if (!moduleBuiltAgainst.valid || !moduleMinimum.valid || !host.valid) {
        return Compatibility::InvalidVersion;
    }
    if (moduleMinimum.compare(moduleBuiltAgainst) > 0) {
        /* minimum newer than the build target: the manifest contradicts itself */
        return Compatibility::InvalidVersion;
    }
    if (moduleBuiltAgainst.major != host.major || moduleMinimum.major != host.major) {
        return Compatibility::MajorMismatch;
    }
    /* host must be at least as new as what the module was compiled against */
    if (host.compare(moduleBuiltAgainst) < 0) return Compatibility::HostTooOld;
    return Compatibility::Compatible;
}

const char* compatibilityToString(Compatibility compatibility) {
    switch (compatibility) {
        case Compatibility::Compatible: return "compatible";
        case Compatibility::MajorMismatch: return "sdk_major_mismatch";
        case Compatibility::HostTooOld: return "host_sdk_too_old";
        case Compatibility::InvalidVersion: return "invalid_version";
    }
    return "unknown";
}

} // namespace aurora
