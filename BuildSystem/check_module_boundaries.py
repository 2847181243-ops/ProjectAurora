#!/usr/bin/env python3
# SPDX-FileCopyrightText: 2026 Project Aurora contributors
# SPDX-License-Identifier: GPL-3.0-or-later

"""Fail the build if a module reaches past the public ABI header.

The architecture claims modules cannot see platform internals. A claim that is
not checked drifts within a week, so this script is part of CI.
"""
import re
import sys
from pathlib import Path

ALLOWED = {"aurora/module_api.h"}


def main():
    root = Path(__file__).resolve().parent.parent
    modules = root / "Modules"
    problems = []
    scanned = 0
    for source in sorted(modules.glob("*/src/*.cpp")):
        scanned += 1
        text = source.read_text(encoding="utf-8")
        for match in re.finditer(r'#\s*include\s*"([^"]+)"', text):
            header = match.group(1)
            if not header.startswith("aurora/"):
                continue
            if header in ALLOWED:
                continue
            problems.append(f"{source.relative_to(root)}: includes {header}")

    if problems:
        print("module boundary violations:")
        for line in problems:
            print("  " + line)
        return 1
    print(f"module boundaries ok ({scanned} source file(s) checked)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
