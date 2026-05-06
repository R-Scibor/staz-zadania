cmake_minimum_required(VERSION 3.15)
project(LogAnalyzer)

set(CMAKE_CXX_STANDARD 20)

find_package(GTest REQUIRED)

add_executable(log_analyzer src/main.cpp src/LogParser.cpp)

add_executable(log_analyzer_tests tests/LogParserTest.cpp src/LogParser.cpp)
target_link_libraries(log_analyzer_tests GTest::gtest_main)
target_include_directories(log_analyzer_tests PRIVATE src)
enable_testing()
add_test(NAME LogParserTests COMMAND log_analyzer_tests)

