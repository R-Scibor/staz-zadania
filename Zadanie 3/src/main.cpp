#include <iostream>
#include <vector>
#include <string>
#include "LogParser.hpp"

void printState(const std::string& file, int entryCount,
                const std::vector<std::string>& sources,
                const std::vector<std::string>& levels,
                const std::vector<std::string>& words,
                const std::string& startTime, const std::string& endTime) {
    std::cout << "\n=== State ===" << std::endl;
    std::cout << "File: " << (file.empty() ? "(none)" : file)
              << " (" << entryCount << " entries)" << std::endl;

    std::cout << "Sources: ";
    if (sources.empty()) std::cout << "(none)";
    for (const auto& s : sources) std::cout << s << " ";
    std::cout << std::endl;

    std::cout << "Levels: ";
    if (levels.empty()) std::cout << "(none)";
    for (const auto& l : levels) std::cout << l << " ";
    std::cout << std::endl;

    std::cout << "Words: ";
    if (words.empty()) std::cout << "(none)";
    for (const auto& w : words) std::cout << w << " ";
    std::cout << std::endl;

    std::cout << "Time: "
              << (startTime.empty() ? "(none)" : startTime)
              << " - "
              << (endTime.empty() ? "(none)" : endTime) << std::endl;

    std::cout << "\n1. Load file\n"
              << "2. Show all entries\n"
              << "3. Show filtered entries\n"
              << "4. Add source filter\n"
              << "5. Add level filter\n"
              << "6. Add word filter\n"
              << "7. Set time range\n"
              << "8. Clear filters\n"
              << "0. Exit\n"
              << "Choice: ";
}

int main() {
    LogParser parser;
    std::string currentFile;
    int entryCount = 0;
    std::vector<std::string> filterSources;
    std::vector<std::string> filterLevels;
    std::vector<std::string> filterWords;
    std::string filterStartTime;
    std::string filterEndTime;

    int choice = -1;
    while (choice != 0) {
        printState(currentFile, entryCount, filterSources, filterLevels,
                   filterWords, filterStartTime, filterEndTime);
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1: {
                std::cout << "File path: ";
                std::getline(std::cin, currentFile);
                try {
                    auto entries = parser.parseLogFile(currentFile);
                    entryCount = entries.size();
                    std::cout << "Loaded " << entryCount << " entries." << std::endl;
                } catch (const std::exception& e) {
                    std::cout << "Error: " << e.what() << std::endl;
                    currentFile = "";
                    entryCount = 0;
                }
                break;
            }
            case 2: {
                if (currentFile.empty()) {
                    std::cout << "Load a file first." << std::endl;
                    break;
                }
                parser.printEntries();
                break;
            }
            case 3: {
                if (currentFile.empty()) {
                    std::cout << "Load a file first." << std::endl;
                    break;
                }
                auto results = parser.filterEntries(filterSources, filterLevels,
                                                    filterWords, filterStartTime, filterEndTime);
                std::cout << "Found " << results.size() << " entries." << std::endl;
                break;
            }
            case 4: {
                std::string val;
                std::cout << "Source: ";
                std::getline(std::cin, val);
                filterSources.push_back(val);
                break;
            }
            case 5: {
                std::string val;
                std::cout << "Level (INFO/WARN/ERROR): ";
                std::getline(std::cin, val);
                filterLevels.push_back(val);
                break;
            }
            case 6: {
                std::string val;
                std::cout << "Word: ";
                std::getline(std::cin, val);
                filterWords.push_back(val);
                break;
            }
            case 7: {
                std::cout << "From (e.g. 2023-10-25T10:00:00): ";
                std::getline(std::cin, filterStartTime);
                std::cout << "To: ";
                std::getline(std::cin, filterEndTime);
                break;
            }
            case 8: {
                filterSources.clear();
                filterLevels.clear();
                filterWords.clear();
                filterStartTime = "";
                filterEndTime = "";
                std::cout << "Filters cleared." << std::endl;
                break;
            }
            case 0:
                break;
            default:
                std::cout << "Invalid choice." << std::endl;
        }
    }
    return 0;
}
