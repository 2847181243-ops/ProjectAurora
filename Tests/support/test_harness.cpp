#include "test_harness.h"

#include <cstdio>

namespace aurora {
namespace test {

int check_count = 0;
int failure_count = 0;
std::string current_test;

std::vector<TestCase>& cases() {
    static std::vector<TestCase> storage;
    return storage;
}

Registrar::Registrar(const char* name, void (*fn)()) { cases().push_back({name, fn}); }

void recordCheck(bool passed, const std::string& description, const char* file, int line) {
    ++check_count;
    if (passed) return;
    ++failure_count;
    std::printf("  FAIL %s:%d  %s  (test: %s)\n", file, line, description.c_str(), current_test.c_str());
}

int runAll() {
    std::printf("running %zu test(s)\n", cases().size());
    int failed_tests = 0;
    for (const auto& test_case : cases()) {
        current_test = test_case.name;
        const int before = failure_count;
        std::printf("TEST %s\n", test_case.name);
        test_case.fn();
        if (failure_count > before) {
            ++failed_tests;
            std::printf("  -> FAILED\n");
        } else {
            std::printf("  -> ok\n");
        }
    }
    std::printf("\n%d check(s), %d failure(s), %d failed test(s)\n",
                check_count, failure_count, failed_tests);
    return failed_tests == 0 ? 0 : 1;
}

} // namespace test
} // namespace aurora
