// SPDX-License-Identifier: GPL-3.0-or-later
//
// Platform hooks ONScripter expects its build to supply. The desktop builds get
// them from platform-specific files we do not compile for iOS.

#include "resources.h"

/* ONScripter can embed files (fonts, icons) into the executable. We ship none,
 * so the lookup always misses and the engine falls back to the data directory. */
const InternalResource* getResource(const char* filename) {
    (void)filename;
    return nullptr;
}
