# Novomatic - Recruitment Tasks

Each subfolder is a self-contained project with its own CMake build, Conan dependencies, sources, and tests.

| Task | Folder | Summary |
|---|---|---|
| 1 | [`Zadanie 1`](./Zadanie%201/README.md) | Ticket-vending kiosk system. TCP client–server, concurrent reservations with TTL, min-coin change from a limited drawer. |
| 2 | [`Zadanie 2`](./Zadanie%202/README.md) | Blueprint graph auto-layout. Layered placement algorithm + ImGui/OpenGL visualizer with pan and zoom. |
| 3 | [`Zadanie 3`](./Zadanie%203/README.md) | Log analyzer. Parses `[TIMESTAMP] [LEVEL] [SOURCE] Message` lines, supports combined filtering with time-range and partial message match. |

## Common setup

All three projects build the same way:

**Requirements:** CMake 3.15+, Conan 2.x, C++20 compiler

```bash
cd "Zadanie N"
conan install . --output-folder=build --build=missing
cmake -B build -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```
