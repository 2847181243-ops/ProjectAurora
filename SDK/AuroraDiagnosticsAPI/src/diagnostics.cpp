// SPDX-FileCopyrightText: 2026 Project Aurora contributors
// SPDX-License-Identifier: GPL-3.0-or-later

#include "aurora/diagnostics.h"

#include <cstdio>

namespace aurora {

const char* logLevelName(AuroraLogLevel level) {
    switch (level) {
        case AURORA_LOG_DEBUG: return "DEBUG";
        case AURORA_LOG_INFO: return "INFO";
        case AURORA_LOG_WARNING: return "WARN";
        case AURORA_LOG_ERROR: return "ERROR";
    }
    return "?";
}

void Diagnostics::record(AuroraLogLevel level, const std::string& moduleId, const std::string& message) {
    LogRecord record;
    record.sequence = nextSequence_++;
    record.level = level;
    record.module_id = moduleId;
    record.message = message;
    record.timestamp_seconds = clock_ ? clock_() : 0.0;
    records_.push_back(std::move(record));
    if (records_.size() > capacity_) records_.pop_front();
    if (level == AURORA_LOG_ERROR) ++errorCount_;
}

std::string Diagnostics::format() const {
    std::string text;
    char buffer[512];
    for (const auto& record : records_) {
        std::snprintf(buffer, sizeof(buffer), "%8.3f %-5s [%s] %s\n",
                      record.timestamp_seconds, logLevelName(record.level),
                      record.module_id.empty() ? "platform" : record.module_id.c_str(),
                      record.message.c_str());
        text += buffer;
    }
    return text;
}

std::string Diagnostics::formatFor(const std::string& moduleId) const {
    std::string text;
    char buffer[512];
    for (const auto& record : records_) {
        if (record.module_id != moduleId) continue;
        std::snprintf(buffer, sizeof(buffer), "%8.3f %-5s %s\n",
                      record.timestamp_seconds, logLevelName(record.level), record.message.c_str());
        text += buffer;
    }
    return text;
}

void Diagnostics::clear() {
    records_.clear();
    errorCount_ = 0;
}

} // namespace aurora
