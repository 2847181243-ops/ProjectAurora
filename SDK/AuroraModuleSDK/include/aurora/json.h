/* Minimal, dependency-free JSON reader.
 *
 * Why hand-rolled: the module runtime may not pull GPL/BSD JSON libraries into an
 * app that also statically links GPL emulators. Fewer licenses to audit, one less
 * thing to break on a platform update. Scope is deliberately small: parse only,
 * complain loudly, never guess.
 */
#ifndef AURORA_JSON_H
#define AURORA_JSON_H

#include <string>
#include <utility>
#include <vector>

namespace aurora {
namespace json {

enum class Type { Null, Bool, Number, String, Array, Object };

struct Value {
    Type type = Type::Null;
    bool boolean = false;
    double number = 0.0;
    std::string string;
    std::vector<Value> array;
    std::vector<std::pair<std::string, Value>> object;

    bool isNull() const { return type == Type::Null; }
    bool isBool() const { return type == Type::Bool; }
    bool isNumber() const { return type == Type::Number; }
    bool isString() const { return type == Type::String; }
    bool isArray() const { return type == Type::Array; }
    bool isObject() const { return type == Type::Object; }

    const Value* find(const std::string& key) const;
    const char* typeName() const;
};

struct ParseError {
    std::size_t offset = 0;
    int line = 1;
    int column = 1;
    std::string message;
};

struct ParseResult {
    bool ok = false;
    Value value;
    ParseError error;
};

/* Parses one JSON value. Trailing non-whitespace input is an error. */
ParseResult parse(const std::string& text);

} // namespace json
} // namespace aurora

#endif
