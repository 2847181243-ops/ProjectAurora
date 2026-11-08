#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""Enforce the project licence policy on module manifests.

The project ships as GPL-3.0-or-later and only accepts modules that can be
redistributed publicly. This check is what makes that a rule instead of a wish:
a manifest whose licence cannot be combined with the app fails CI.
"""
import json
import sys
from pathlib import Path

# Licences that can be combined into a GPLv3 work and redistributed publicly.
ALLOWED = {
    "GPL-2.0-or-later",
    "GPL-3.0-only",
    "GPL-3.0-or-later",
    "LGPL-2.1-or-later",
    "LGPL-3.0-or-later",
    "MPL-2.0",
    "Apache-2.0",
    "MIT",
    "BSD-2-Clause",
    "BSD-3-Clause",
    "ISC",
    "Zlib",
    "CC0-1.0",
    "Unlicense",
}

# Explicitly refused, with the reason written out so the fix is obvious.
REFUSED = {
    "GPL-2.0-only": "GPLv2-only cannot be combined with this GPLv3 work",
}


def main():
    root = Path(__file__).resolve().parent.parent
    manifests = sorted((root / "ModuleManifests").glob("*.json"))
    problems = []
    for manifest in manifests:
        try:
            data = json.loads(manifest.read_text(encoding="utf-8"))
        except json.JSONDecodeError as error:
            problems.append(f"{manifest.name}: invalid JSON ({error})")
            continue
        licence = str(data.get("license", "")).strip()
        if not licence:
            problems.append(f"{manifest.name}: missing license field")
            continue
        if licence in REFUSED:
            problems.append(f"{manifest.name}: {licence} - {REFUSED[licence]}")
            continue
        if licence in ALLOWED:
            continue
        if "-NC" in licence or "-ND" in licence:
            problems.append(
                f"{manifest.name}: {licence} forbids commercial use or modification; "
                "the project only ships modules that can be redistributed publicly"
            )
            continue
        problems.append(f"{manifest.name}: {licence} is not on the allow list")

    if problems:
        print("licence policy violations:")
        for line in problems:
            print("  " + line)
        return 1
    print(f"licence policy ok ({len(manifests)} manifest(s) checked)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
