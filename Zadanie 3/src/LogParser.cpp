#include "LogParser.hpp"

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
    logEntries = entries;
    return entries;
}

void LogParser::printEntry(LogEntry entry)
{
    std::cout << entry.timestamp << " " << entry.logLevel << " " << entry.source << " " << entry.message << std::endl;
}

void LogParser::printEntries(const std::vector<LogEntry>& entries) {
    if (entries.empty()) {
        for (const auto& entry : logEntries) {
            printEntry(entry);
        }
    } else {
        for (const auto& entry : entries) {
            printEntry(entry);
        }
    }
}

std::vector<LogEntry> LogParser::filterByLevel(std::vector<LogEntry> entries, const std::vector<std::string>& levels) {
    std::vector<LogEntry> filteredEntries;
    for (const auto& entry : entries) {
        if (std::find(levels.begin(), levels.end(), entry.logLevel) != levels.end()) {
            filteredEntries.push_back(entry);
        }
    }
    return filteredEntries;
}

std::vector<LogEntry> LogParser::filterBySource(std::vector<LogEntry> entries, const std::vector<std::string>& sources) {
    std::vector<LogEntry> filteredEntries;
    for (const auto& entry : entries) {
        if (std::find(sources.begin(), sources.end(), entry.source) != sources.end()) {
            filteredEntries.push_back(entry);
        }
    }
    return filteredEntries;
}

std::vector<LogEntry> LogParser::filterByMessage(std::vector<LogEntry> entries, const std::vector<std::string>& words) {
    std::vector<LogEntry> filteredEntries;
    // TODO filter words and message to lower case ???
    for (const auto& entry : entries) {
        for (const auto& word : words) {
            if (entry.message.find(word) != std::string::npos){
                filteredEntries.push_back(entry);
                break;
            }
        }
    }
    return filteredEntries;
}

std::vector<LogEntry> LogParser::filterByTimestamp(std::vector<LogEntry> entries, const std::string& startTime, const std::string& endTime) {
    std::vector<LogEntry> filteredEntries;
    for (const auto& entry : entries) {
        if (!startTime.empty() && entry.timestamp < startTime) continue;
        if (!endTime.empty() && entry.timestamp > endTime) continue;
        filteredEntries.push_back(entry);

    }
    return filteredEntries;
}

std::vector<LogEntry> LogParser::filterEntries(const std::vector<std::string>& sources, const std::vector<std::string>& levels,
                                                 const std::vector<std::string>& words, const std::string& startTime, const std::string& endTime) {
    std::vector<LogEntry> filteredEntries;
    if (!sources.empty()) {
        filteredEntries = filterBySource(logEntries, sources);
    }
    else {
        filteredEntries = logEntries;
    }
    if (!levels.empty()) {
        filteredEntries = filterByLevel(filteredEntries, levels);
    }
    if (!words.empty()) {
        filteredEntries = filterByMessage(filteredEntries, words);
    }
    if (startTime!= "" || endTime!= "") {
        filteredEntries = filterByTimestamp(filteredEntries, startTime, endTime);
    }
    for (const auto& entry : filteredEntries) { 
        printEntry(entry);
    }
    return filteredEntries;

}