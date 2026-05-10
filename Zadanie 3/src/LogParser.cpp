#include "LogParser.hpp"

#include <algorithm>

static size_t lowerBoundByTimestamp(const std::vector<LogEntry>& entries, const std::string& target) {
    size_t lo = 0;
    size_t hi = entries.size();
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (entries[mid].timestamp < target) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

static size_t upperBoundByTimestamp(const std::vector<LogEntry>& entries, const std::string& target) {
    size_t lo = 0;
    size_t hi = entries.size();
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (entries[mid].timestamp <= target) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

std::vector<LogEntry> LogParser::parseLogFile(const std::string& filePath) {
    std::vector<LogEntry> entries;
    std::ifstream file(filePath);

    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filePath);
    }

    std::string line;
    while (std::getline(file, line)) {
        LogEntry entry;
        size_t pos1 = line.find(' ');
        size_t pos2 = line.find(' ', pos1 + 1);
        size_t pos3 = line.find(' ', pos2 + 1);


        if (pos1 == std::string::npos || pos2 == std::string::npos || pos3 == std::string::npos) {
            continue; // Skip malformed lines
        }

        if (line[0] != '[' || line[pos1 - 1] != ']' || line[pos1 + 1] != '[' || line[pos2 + 1] != '[' || line[pos2 - 1] != ']' || line[pos3 - 1] != ']') {
            continue; // Skip lines that don't match the expected format
        }

        entry.timestamp = line.substr(1, pos1 - 2);
        entry.logLevel = line.substr(pos1 + 2, pos2 - pos1 - 3);
        entry.source = line.substr(pos2 + 2, pos3 - pos2 - 3);
        entry.message = line.substr(pos3 + 1);

        entries.push_back(entry);
    }
    std::sort(entries.begin(), entries.end(),
              [](const LogEntry& a, const LogEntry& b) { return a.timestamp < b.timestamp; });
    logEntries = entries;
    return entries;
}

void LogParser::printEntry(LogEntry entry)
{
    std::cout << "[" << entry.timestamp << "] [" << entry.logLevel << "] [" << entry.source << "] " << entry.message << std::endl;
}

void LogParser::printEntries(const std::vector<LogEntry>& entries) {
    for (const auto& entry : entries) {
        printEntry(entry);
    }
}

void LogParser::printAll() {
    printEntries(logEntries);
}

// Returns true if entry passes every active filter category.
// An empty list for a category means that category is inactive.
static bool matches(const LogEntry& entry,
                    const std::vector<std::string>& sources,
                    const std::vector<std::string>& levels,
                    const std::vector<std::string>& words) {
    if (!sources.empty() &&
        std::find(sources.begin(), sources.end(), entry.source) == sources.end()) {
        return false;
    }
    if (!levels.empty() &&
        std::find(levels.begin(), levels.end(), entry.logLevel) == levels.end()) {
        return false;
    }
    if (!words.empty()) {
        bool anyWordHit = false;
        for (const auto& word : words) {
            if (entry.message.find(word) != std::string::npos) {
                anyWordHit = true;
                break;
            }
        }
        if (!anyWordHit) return false;
    }
    return true;
}

// Expects entries sorted by timestamp
std::vector<LogEntry> LogParser::filterByTimestamp(std::vector<LogEntry> entries, const std::string& startTime, const std::string& endTime) {
    size_t startIdx = 0;
    size_t endIdx = entries.size();
    if (!startTime.empty()) startIdx = lowerBoundByTimestamp(entries, startTime);
    if (!endTime.empty())   endIdx   = upperBoundByTimestamp(entries, endTime);
    return std::vector<LogEntry>(entries.begin() + startIdx, entries.begin() + endIdx);
}

std::vector<LogEntry> LogParser::filterEntries(const std::vector<std::string>& sources, const std::vector<std::string>& levels,
                                                 const std::vector<std::string>& words, const std::string& startTime, const std::string& endTime) {
    std::vector<LogEntry> timeSlice = filterByTimestamp(logEntries, startTime, endTime);
    std::vector<LogEntry> result;
    for (const auto& entry : timeSlice) {
        if (matches(entry, sources, levels, words)) {
            result.push_back(entry);
        }
    }
    return result;
}