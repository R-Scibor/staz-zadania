# Graph Renderer

A blueprint-style graph viewer. Reads a directed graph from JSON, computes a layered layout, and renders nodes and edges with ImGui/OpenGL. Pan and zoom with the mouse.

## Input format

```jsonc
{
  "nodes": [
    { "id": 1, "name": "Event BeginPlay", "x": 0, "y": 0 },
    { "id": 2, "name": "Branch",          "x": 0, "y": 0 }
  ],
  "edges": [
    { "from": 1, "to": 2 }
  ]
}
```

`x`/`y` on input are ignored. The computed positions are written to `output.json`.

## Build

**Requirements:** CMake 3.15+, Conan 2.x, C++20 compiler

```bash
conan install . --output-folder=build --build=missing
cmake -B build -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

## Run

```bash
./build/Release/graph_renderer
```

Left-drag pans. Wheel zooms toward the cursor (clamped 0.1×–5×).

## Tests

```bash
./build/Release/json_parser_tests
./build/Release/graph_tests
```

## Engine notes

Layout runs in two passes.

`assignLayers` walks from the roots (nodes with no parents) via a worklist. Each node's layer is the longest path from any root — a child gets pushed deeper whenever a longer path is found, and re-queued so its own descendants follow. This keeps every edge pointing left-to-right. Cycles are detected by counting pops per node and abort with a warning.

`calculateNodePositions` turns each layer into an x-column. Within a layer, each node's sort key is the mean y of its parents (roots use 0); nodes are then placed in evenly-spaced y-slots in that order, centered on the canvas. Children get pulled toward their parents' vertical band, but never share a slot — so siblings of a single parent end up offset rather than literally midway between parents.
