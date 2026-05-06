#include <gtest/gtest.h>
#include "LogParser.hpp"

TEST(LogParserTest, ParseReturnsCorrectCount) {
    LogParser parser;
    auto entries = parser.parseLogFile("logs.txt");
    EXPECT_EQ(entries.size(), 5);
}

TEST(LogParserTest, FilterByLevelReturnsCorrectEntries) {
    LogParser parser;
    parser.parseLogFile("logs.txt");
    std::vector<std::string> levels = {"ERROR", "WARN"};
    std::vector<std::string> words = {};
    std::vector<std::string> sources = {};
    auto filteredEntries = parser.filterEntries(sources, levels, words);
    EXPECT_EQ(filteredEntries.size(), 3);
    for (const auto& entry : filteredEntries) {
        EXPECT_TRUE(entry.logLevel == "ERROR" || entry.logLevel == "WARN");
    }
}

TEST(LogParserTest, FilterByMessageReturnsCorrectEntries) {
    LogParser parser;
    parser.parseLogFile("logs.txt");
    std::vector<std::string> levels = {};
    std::vector<std::string> words = {"User", "Transaction"};
    std::vector<std::string> sources = {};
    auto filteredEntries = parser.filterEntries(sources, levels, words);
    EXPECT_EQ(filteredEntries.size(), 3);
    for (const auto& entry : filteredEntries) {
        EXPECT_TRUE(entry.message.find("User") != std::string::npos || entry.message.find("Transaction") != std::string::npos);
    }
}

TEST(LogParserTest, FilterByMessageReturnsCorrectEntry) {
    LogParser parser;
    parser.parseLogFile("logs.txt");
    std::vector<std::string> levels = {};
    std::vector<std::string> words = {"Transaction"};
    std::vector<std::string> sources = {};
    auto filteredEntries = parser.filterEntries(sources, levels, words);
    EXPECT_EQ(filteredEntries.size(), 2);
    for (const auto& entry : filteredEntries) {
        EXPECT_TRUE(entry.message.find("Transaction") != std::string::npos);
    }
}

TEST(LogParserTest, FilterBySourceReturnsCorrectEntries) {
    LogParser parser;
    parser.parseLogFile("logs.txt");
    std::vector<std::string> levels = {};
    std::vector<std::string> words = {};
    std::vector<std::string> sources = {"AuthService", "Payment"};
    auto filteredEntries = parser.filterEntries(sources, levels, words);
    EXPECT_EQ(filteredEntries.size(), 4);
    for (const auto& entry : filteredEntries) {
        EXPECT_TRUE(entry.source == "AuthService" || entry.source == "Payment");
    }
}

TEST(LogParserTest, FilterByTimeReturnsCorrectEntries) {
    LogParser parser;
    parser.parseLogFile("logs.txt");
    std::vector<std::string> levels = {};
    std::vector<std::string> words = {};
    std::vector<std::string> sources = {};
    auto filteredEntries = parser.filterEntries(sources, levels, words, "2023-10-25T10:05:12", "2023-10-25T10:20:00");
    EXPECT_EQ(filteredEntries.size(), 3);
}

TEST(LogParserTest, FilterByAllReturnsCorrectEntries) {
    LogParser parser;
    parser.parseLogFile("logs.txt");
    std::vector<std::string> levels = {"ERROR", "WARN"};
    std::vector<std::string> words = {"User", "Transaction"};
    std::vector<std::string> sources = {"AuthService", "Payment"};
    auto filteredEntries = parser.filterEntries(sources, levels, words, "2023-10-25T10:05:12", "2023-10-25T10:20:00");
    EXPECT_EQ(filteredEntries.size(), 1);
    for (const auto& entry : filteredEntries) {
        EXPECT_TRUE(entry.logLevel == "ERROR" || entry.logLevel == "WARN");
        EXPECT_TRUE(entry.source == "AuthService" || entry.source == "Payment");
        EXPECT_TRUE(entry.message.find("User") != std::string::npos || entry.message.find("Transaction") != std::string::npos);
        EXPECT_TRUE(entry.timestamp >= "2023-10-25T10:05:12" && entry.timestamp <= "2023-10-25T10:20:00");
    }
}
