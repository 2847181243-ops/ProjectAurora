// SPDX-FileCopyrightText: 2026 Project Aurora contributors
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef AURORA_TEST_HARNESS_H
#define AURORA_TEST_HARNESS_H

#include <sstream>
#include <string>
#include <vector>

#include "aurora/manifest.h"
#include "aurora/module_api.h"

namespace aurora {
namespace test {

struct TestCase {
    const char* name;
    void (*fn)();
};

std::vector<TestCase>& cases();
struct Registrar {
    Registrar(const char* name, void (*fn)());
};

extern int check_count;
extern int failure_count;
extern std::string current_test;

void recordCheck(bool passed, const std::string& description, const char* file, int line);
int runAll();

template <typename T>
std::string describe(const T& value) {
    std::ostringstream stream;
    stream << value;
    return stream.str();
}
inline std::string describe(bool value) { return value ? "true" : "false"; }
inline std::string describe(const std::string& value) { return "\"" + value + "\""; }
inline std::string describe(const char* value) { return std::string("\"") + (value ? value : "(null)") + "\""; }
inline std::string describe(AuroraModuleState value) { return aurora_module_state_string(value); }
inline std::string describe(AuroraStatus value) { return aurora_status_string(value); }
inline std::string describe(AdapterKind value) { return adapterKindName(value); }

template <typename A, typename B>
void checkEqual(const A& left, const B& right, const char* expression, const char* file, int line) {
    const bool same = left == right;
    std::string description = expression;
    if (!same) {
        description += "  [left = " + describe(left) + ", right = " + describe(right) + "]";
    }
    recordCheck(same, description, file, line);
}

template <typename A>
void checkContains(const A& haystack, const char* needle, const char* expression, const char* file, int line) {
    const bool found = describe(haystack).find(needle) != std::string::npos;
    std::string description = expression;
    if (!found) description += std::string("  [missing ") + needle + "]";
    recordCheck(found, description, file, line);
}

} // namespace test
} // namespace aurora

#define AURORA_TEST(name)                                                   \
    static void name();                                                     \
    static ::aurora::test::Registrar aurora_registrar_##name(#name, &name); \
    static void name()

#define CHECK(expression) \
    ::aurora::test::recordCheck((expression) ? true : false, #expression, __FILE__, __LINE__)

#define CHECK_EQ(left, right) \
    ::aurora::test::checkEqual((left), (right), #left " == " #right, __FILE__, __LINE__)

#define CHECK_CONTAINS(haystack, needle) \
    ::aurora::test::checkContains((haystack), (needle), #haystack " contains " #needle, __FILE__, __LINE__)

#endif
