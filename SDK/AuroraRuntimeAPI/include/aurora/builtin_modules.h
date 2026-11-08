// SPDX-FileCopyrightText: 2026 Project Aurora contributors
// SPDX-License-Identifier: GPL-3.0-or-later

/* Generated build table: which modules this build actually linked.
 *
 * This file is the second of the three inputs the registry fuses. It is produced
 * by CMake from the enabled module targets, so a module that failed to compile or
 * was switched off simply is not in the table - and the platform says so in the
 * UI instead of pretending the module exists.
 */
#ifndef AURORA_BUILTIN_MODULES_H
#define AURORA_BUILTIN_MODULES_H

#include <cstddef>

#include "aurora/module_api.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct AuroraBuiltinModule {
    const char* module_id;
    AuroraModuleEntryFn entry;
} AuroraBuiltinModule;

extern const AuroraBuiltinModule aurora_builtin_modules[];
extern const size_t aurora_builtin_module_count;

#ifdef __cplusplus
}
#endif

#endif /* AURORA_BUILTIN_MODULES_H */
