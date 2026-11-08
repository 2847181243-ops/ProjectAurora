// SPDX-FileCopyrightText: 2026 Project Aurora contributors
// SPDX-License-Identifier: GPL-3.0-or-later

#include "aurora/storage.h"

#include <filesystem>
#include <vector>

namespace aurora {

namespace {
bool isLowerAlpha(char c) { return c >= 'a' && c <= 'z'; }
bool isDigit(char c) { return c >= '0' && c <= '9'; }
} // namespace

StorageLayout::StorageLayout(std::string rootDirectory) : root_(std::move(rootDirectory)) {}

bool StorageLayout::isSafeModuleId(const std::string& moduleId) {
    if (moduleId.empty() || moduleId.size() > 128) return false;
    if (!isLowerAlpha(moduleId[0])) return false;
    for (char c : moduleId) {
        if (isLowerAlpha(c) || isDigit(c) || c == '.' || c == '_') continue;
        return false;
    }
    if (moduleId.find("..") != std::string::npos) return false;
    return true;
}

bool StorageLayout::isSafeRelativePath(const std::string& relativePath) {
    if (relativePath.empty()) return true;
    if (relativePath.size() > 512) return false;
    if (relativePath[0] == '/' || relativePath[0] == '\\') return false;
    if (relativePath.find('\\') != std::string::npos) return false;
    if (relativePath.find(':') != std::string::npos) return false;
    std::size_t start = 0;
    while (start <= relativePath.size()) {
        const std::size_t slash = relativePath.find('/', start);
        const std::string segment = relativePath.substr(
            start, slash == std::string::npos ? std::string::npos : slash - start);
        if (segment == "." || segment == "..") return false;
        if (segment.empty() && slash != std::string::npos) return false;
        if (slash == std::string::npos) break;
        start = slash + 1;
    }
    return true;
}

std::string StorageLayout::bucketDirectory(const std::string& moduleId, AuroraStorageKind kind) const {
    if (!isSafeModuleId(moduleId)) return std::string();
    std::filesystem::path base(root_);
    base /= "RuntimeModules";
    base /= moduleId;
    base /= aurora_storage_kind_string(kind);
    return base.generic_string();
}

std::string StorageLayout::resolve(const std::string& moduleId,
                                   AuroraStorageKind kind,
                                   const std::string& relativePath,
                                   std::string& error) const {
    error.clear();
    if (!isSafeModuleId(moduleId)) {
        error = "unsafe module id";
        return std::string();
    }
    if (!isSafeRelativePath(relativePath)) {
        error = "relative path escapes the module tree";
        return std::string();
    }
    std::filesystem::path result(bucketDirectory(moduleId, kind));
    if (!relativePath.empty()) result /= std::filesystem::path(relativePath).lexically_normal();
    if (result.empty()) {
        error = "empty path";
        return std::string();
    }
    const std::filesystem::path bucket(bucketDirectory(moduleId, kind));
    /* Belt and braces: after normalization the result must still sit inside the
     * bucket. lexical checks above should already guarantee it. */
    const std::string normalized = result.lexically_normal().generic_string();
    const std::string bucketPrefix = bucket.lexically_normal().generic_string();
    if (normalized.compare(0, bucketPrefix.size(), bucketPrefix) != 0) {
        error = "relative path escapes the module tree";
        return std::string();
    }
    return normalized;
}

bool StorageLayout::ensureBucket(const std::string& moduleId, AuroraStorageKind kind,
                                 std::string& error) const {
    const std::string directory = bucketDirectory(moduleId, kind);
    if (directory.empty()) {
        error = "unsafe module id";
        return false;
    }
    std::error_code code;
    std::filesystem::create_directories(directory, code);
    if (code) {
        error = "cannot create directory: " + code.message();
        return false;
    }
    error.clear();
    return true;
}

} // namespace aurora
