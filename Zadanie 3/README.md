# Log Analyzer

A console application for parsing, filtering, and analyzing system log files.

## Log format

Each line must follow this structure:
```
[TIMESTAMP] [LOG_LEVEL] [SOURCE] Message
```

Example:
```
[2023-10-25T10:00:00] [INFO] [AuthService] User logged in successfully
```

## Build

**Requirements:** CMake 3.15+, Conan 2.x, C++20 compiler

```bash
conan install . --output-folder=build --build=missing
cmake -B build -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

## Run

```bash
./build/Release/log_analyzer
```

The interactive menu allows loading a log file and filtering entries by source, level, message content, and time range — individually or in any combination.

## Tests

```bash
./build/Release/log_analyzer_tests
```
