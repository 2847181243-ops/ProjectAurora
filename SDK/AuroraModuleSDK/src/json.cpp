// SPDX-FileCopyrightText: 2026 Project Aurora contributors
// SPDX-License-Identifier: GPL-3.0-or-later

#include "aurora/json.h"

#include <cstdio>
#include <cstdlib>

namespace aurora {
namespace json {

const Value* Value::find(const std::string& key) const {
    if (type != Type::Object) return nullptr;
    for (const auto& kv : object) {
        if (kv.first == key) return &kv.second;
    }
    return nullptr;
}

const char* Value::typeName() const {
    switch (type) {
        case Type::Null: return "null";
        case Type::Bool: return "boolean";
        case Type::Number: return "number";
        case Type::String: return "string";
        case Type::Array: return "array";
        case Type::Object: return "object";
    }
    return "?";
}

namespace {

class Parser {
public:
    explicit Parser(const std::string& text) : text_(text) {}

    bool run(Value& out, ParseError& error) {
        skipSpace();
        if (!parseValue(out, 0)) { error = error_; return false; }
        skipSpace();
        if (pos_ != text_.size()) {
            fail("unexpected trailing characters");
            error = error_;
            return false;
        }
        return true;
    }

private:
    void fail(const std::string& message) {
        if (!failed_) {
            failed_ = true;
            error_.message = message;
            error_.offset = pos_;
            computeLineColumn();
        }
    }

    void computeLineColumn() {
        int line = 1;
        int column = 1;
        for (std::size_t i = 0; i < error_.offset && i < text_.size(); ++i) {
            if (text_[i] == '\n') { ++line; column = 1; } else { ++column; }
        }
        error_.line = line;
        error_.column = column;
    }

    bool eof() const { return pos_ >= text_.size(); }
    char peek() const { return eof() ? '\0' : text_[pos_]; }

    void skipSpace() {
        while (!eof()) {
            const char c = text_[pos_];
            if (c == ' ' || c == '\t' || c == '\n' || c == '\r') { ++pos_; continue; }
            break;
        }
    }

    bool expect(char c) {
        if (peek() != c) {
            char buf[64];
            std::snprintf(buf, sizeof(buf), "expected '%c'", c);
            fail(buf);
            return false;
        }
        ++pos_;
        return true;
    }

    bool literal(const char* word) {
        std::size_t n = 0;
        while (word[n] != '\0') ++n;
        if (text_.compare(pos_, n, word) != 0) { fail("invalid literal"); return false; }
        pos_ += n;
        return true;
    }

    bool parseValue(Value& out, int depth) {
        if (depth > 64) { fail("nesting too deep"); return false; }
        skipSpace();
        if (eof()) { fail("unexpected end of input"); return false; }
        const char c = peek();
        switch (c) {
            case '{': return parseObject(out, depth);
            case '[': return parseArray(out, depth);
            case '"': {
                out.type = Type::String;
                return parseString(out.string);
            }
            case 't': out.type = Type::Bool; out.boolean = true; return literal("true");
            case 'f': out.type = Type::Bool; out.boolean = false; return literal("false");
            case 'n': out.type = Type::Null; return literal("null");
            default:
                if (c == '-' || (c >= '0' && c <= '9')) return parseNumber(out);
                fail("unexpected character");
                return false;
        }
    }

    bool parseObject(Value& out, int depth) {
        out.type = Type::Object;
        if (!expect('{')) return false;
        skipSpace();
        if (peek() == '}') { ++pos_; return true; }
        for (;;) {
            skipSpace();
            std::string key;
            if (!parseString(key)) return false;
            skipSpace();
            if (!expect(':')) return false;
            Value item;
            if (!parseValue(item, depth + 1)) return false;
            if (out.find(key) != nullptr) {
                fail("duplicate key '" + key + "'");
                return false;
            }
            out.object.emplace_back(std::move(key), std::move(item));
            skipSpace();
            if (peek() == ',') { ++pos_; continue; }
            if (peek() == '}') { ++pos_; return true; }
            fail("expected ',' or '}'");
            return false;
        }
    }

    bool parseArray(Value& out, int depth) {
        out.type = Type::Array;
        if (!expect('[')) return false;
        skipSpace();
        if (peek() == ']') { ++pos_; return true; }
        for (;;) {
            Value item;
            if (!parseValue(item, depth + 1)) return false;
            out.array.emplace_back(std::move(item));
            skipSpace();
            if (peek() == ',') { ++pos_; continue; }
            if (peek() == ']') { ++pos_; return true; }
            fail("expected ',' or ']'");
            return false;
        }
    }

    static void appendUtf8(std::string& out, unsigned cp) {
        if (cp <= 0x7F) {
            out.push_back(static_cast<char>(cp));
        } else if (cp <= 0x7FF) {
            out.push_back(static_cast<char>(0xC0 | (cp >> 6)));
            out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
        } else if (cp <= 0xFFFF) {
            out.push_back(static_cast<char>(0xE0 | (cp >> 12)));
            out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
        } else {
            out.push_back(static_cast<char>(0xF0 | (cp >> 18)));
            out.push_back(static_cast<char>(0x80 | ((cp >> 12) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
        }
    }

    bool parseHex4(unsigned& out) {
        out = 0;
        for (int i = 0; i < 4; ++i) {
            if (eof()) { fail("truncated \\u escape"); return false; }
            const char c = text_[pos_++];
            unsigned digit = 0;
            if (c >= '0' && c <= '9') digit = static_cast<unsigned>(c - '0');
            else if (c >= 'a' && c <= 'f') digit = static_cast<unsigned>(c - 'a' + 10);
            else if (c >= 'A' && c <= 'F') digit = static_cast<unsigned>(c - 'A' + 10);
            else { fail("bad hex digit in \\u escape"); return false; }
            out = (out << 4) | digit;
        }
        return true;
    }

    bool parseString(std::string& out) {
        if (!expect('"')) return false;
        out.clear();
        for (;;) {
            if (eof()) { fail("unterminated string"); return false; }
            const char c = text_[pos_++];
            if (c == '"') return true;
            if (static_cast<unsigned char>(c) < 0x20) { fail("control character in string"); return false; }
            if (c != '\\') { out.push_back(c); continue; }
            if (eof()) { fail("unterminated escape"); return false; }
            const char e = text_[pos_++];
            switch (e) {
                case '"': out.push_back('"'); break;
                case '\\': out.push_back('\\'); break;
                case '/': out.push_back('/'); break;
                case 'b': out.push_back('\b'); break;
                case 'f': out.push_back('\f'); break;
                case 'n': out.push_back('\n'); break;
                case 'r': out.push_back('\r'); break;
                case 't': out.push_back('\t'); break;
                case 'u': {
                    unsigned cp = 0;
                    if (!parseHex4(cp)) return false;
                    if (cp >= 0xD800 && cp <= 0xDBFF) {
                        if (pos_ + 1 < text_.size() && text_[pos_] == '\\' && text_[pos_ + 1] == 'u') {
                            pos_ += 2;
                            unsigned low = 0;
                            if (!parseHex4(low)) return false;
                            if (low >= 0xDC00 && low <= 0xDFFF) {
                                cp = 0x10000 + ((cp - 0xD800) << 10) + (low - 0xDC00);
                            } else {
                                fail("invalid low surrogate");
                                return false;
                            }
                        } else {
                            fail("lone high surrogate");
                            return false;
                        }
                    }
                    appendUtf8(out, cp);
                    break;
                }
                default:
                    fail("unknown escape sequence");
                    return false;
            }
        }
    }

    bool parseNumber(Value& out) {
        const std::size_t start = pos_;
        if (peek() == '-') ++pos_;
        if (eof()) { fail("truncated number"); return false; }
        if (peek() == '0') {
            ++pos_;
        } else if (peek() >= '1' && peek() <= '9') {
            while (!eof() && peek() >= '0' && peek() <= '9') ++pos_;
        } else {
            fail("invalid number");
            return false;
        }
        if (peek() == '.') {
            ++pos_;
            if (eof() || peek() < '0' || peek() > '9') { fail("invalid fraction"); return false; }
            while (!eof() && peek() >= '0' && peek() <= '9') ++pos_;
        }
        if (peek() == 'e' || peek() == 'E') {
            ++pos_;
            if (peek() == '+' || peek() == '-') ++pos_;
            if (eof() || peek() < '0' || peek() > '9') { fail("invalid exponent"); return false; }
            while (!eof() && peek() >= '0' && peek() <= '9') ++pos_;
        }
        const std::string literalText = text_.substr(start, pos_ - start);
        out.type = Type::Number;
        out.number = std::strtod(literalText.c_str(), nullptr);
        return true;
    }

    const std::string& text_;
    std::size_t pos_ = 0;
    bool failed_ = false;
    ParseError error_;
};

} // namespace

ParseResult parse(const std::string& text) {
    ParseResult result;
    Value value;
    ParseError error;
    Parser parser(text);
    if (parser.run(value, error)) {
        result.ok = true;
        result.value = std::move(value);
    } else {
        result.ok = false;
        result.error = error;
    }
    return result;
}

} // namespace json
} // namespace aurora
