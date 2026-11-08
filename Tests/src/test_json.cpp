// SPDX-FileCopyrightText: 2026 Project Aurora contributors
// SPDX-License-Identifier: GPL-3.0-or-later

#include <string>

#include "aurora/json.h"
#include "test_harness.h"

using namespace aurora;

AURORA_TEST(json_parses_nested_values) {
    const std::string text = R"({
        "name": "aurora",
        "version": 1.5,
        "flags": [true, false, null],
        "nested": {"empty_object": {}, "empty_array": []},
        "escape": "line\nbreak\u00e9"
    })";
    const json::ParseResult result = json::parse(text);
    CHECK(result.ok);
    if (!result.ok) return;
    CHECK(result.value.isObject());
    const json::Value* name = result.value.find("name");
    CHECK(name != nullptr);
    if (name != nullptr) CHECK_EQ(name->string, std::string("aurora"));
    const json::Value* version = result.value.find("version");
    CHECK(version != nullptr && version->isNumber());
    const json::Value* flags = result.value.find("flags");
    CHECK(flags != nullptr && flags->isArray());
    if (flags != nullptr) {
        CHECK_EQ(flags->array.size(), static_cast<std::size_t>(3));
        CHECK(flags->array[0].boolean);
    }
    const json::Value* nested = result.value.find("nested");
    CHECK(nested != nullptr && nested->isObject());
}

AURORA_TEST(json_rejects_malformed_input) {
    {
        const json::ParseResult trailing_comma = json::parse(R"({"a": 1,})");
        CHECK(!trailing_comma.ok);
    }
    {
        const json::ParseResult unterminated = json::parse(R"({"a": 1)");
        CHECK(!unterminated.ok);
    }
    {
        const json::ParseResult duplicate = json::parse(R"({"a": 1, "a": 2})");
        CHECK(!duplicate.ok);
        CHECK_CONTAINS(duplicate.error.message, "duplicate");
    }
    {
        const json::ParseResult trailing_garbage = json::parse(R"({} extra)");
        CHECK(!trailing_garbage.ok);
    }
    {
        const json::ParseResult bad_escape = json::parse(R"("bad \q escape")");
        CHECK(!bad_escape.ok);
    }
    {
        const json::ParseResult empty = json::parse("");
        CHECK(!empty.ok);
    }
}
