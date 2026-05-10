#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>


struct LogEntry {
    std::string timestamp;
    std::string logLevel;
    std::string source;
    std::string message;
};

class LogParser {
private:
    std::vector<LogEntry> logEntries;
    std::vector<LogEntry> filterByTimestamp(std::vector<LogEntry> entries, const std::string& startTime, const std::string& endTime);

public:
    std::vector<LogEntry> parseLogFile(const std::string& filePath);
    std::vector<LogEntry> filterEntries(const std::vector<std::string>& sources, const std::vector<std::string>& levels, const std::vector<std::string>& words,
                                             const std::string& startTime = "", const std::string& endTime = "");
    void printEntry(LogEntry entry);
    void printEntries(const std::vector<LogEntry>& entries);
    void printAll();
};
