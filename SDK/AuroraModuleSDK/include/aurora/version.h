// SPDX-FileCopyrightText: 2026 Project Aurora contributors
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef AURORA_VERSION_H
#define AURORA_VERSION_H

#include <string>

namespace aurora {

/* SemVer 2.0 subset: MAJOR.MINOR.PATCH with an optional -prerelease suffix.
 * Build metadata is accepted and ignored. */
struct SemVer {
    int major = 0;
    int minor = 0;
    int patch = 0;
    std::string prerelease;
    bool valid = false;

    static bool parse(const std::string& text, SemVer& out);
    int compare(const SemVer& other) const;
    bool isPrerelease() const { return !prerelease.empty(); }
    std::string toString() const;
};

/* Compatibility rule for SDK versions.
 *  - major must match exactly (a breaking SDK change requires a new module build)
 *  - the host minor must be >= the module's minimum minor *and* >= the minor the
 *    module was built against, otherwise the module could call functions that do
 *    not exist in the host. */
enum class Compatibility { Compatible, MajorMismatch, HostTooOld, InvalidVersion };
Compatibility checkSdkCompatibility(const SemVer& moduleBuiltAgainst,
                                    const SemVer& moduleMinimum,
                                    const SemVer& host);

const char* compatibilityToString(Compatibility compatibility);

} // namespace aurora

#endif
