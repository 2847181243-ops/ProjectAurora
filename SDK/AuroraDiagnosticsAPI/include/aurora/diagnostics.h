// SPDX-FileCopyrightText: 2026 Project Aurora contributors
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef AURORA_DIAGNOSTICS_H
#define AURORA_DIAGNOSTICS_H

#include <cstdint>
#include <deque>
#include <string>

#include "aurora/module_api.h"

namespace aurora {

struct LogRecord {
    std::uint64_t sequence = 0;
    AuroraLogLevel level = AURORA_LOG_INFO;
    std::string module_id;
    std::string message;
    double timestamp_seconds = 0.0;
};

/* One sink per app. Modules never write files for logging themselves; they call
 * host->write_log and the platform decides where it lands. */
class Diagnostics {
public:
    explicit Diagnostics(std::size_t capacity = 2048) : capacity_(capacity) {}

    void record(AuroraLogLevel level, const std::string& moduleId, const std::string& message);
    void setClockSource(double (*clock)()) { clock_ = clock; }

    const std::deque<LogRecord>& records() const { return records_; }
    std::size_t errorCount() const { return errorCount_; }
    std::string format() const;                 /* every record, one per line */
    std::string formatFor(const std::string& moduleId) const;
    void clear();

private:
    std::deque<LogRecord> records_;
    std::size_t capacity_;
    std::size_t errorCount_ = 0;
    std::uint64_t nextSequence_ = 0;
    double (*clock_)() = nullptr;
};

const char* logLevelName(AuroraLogLevel level);

} // namespace aurora

#endif
