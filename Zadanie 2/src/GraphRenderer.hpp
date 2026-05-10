#pragma once
#include "Graph.hpp"
#include "NodeStruct.hpp"
#include "Config.hpp"
#include "imgui.h"
#include <unordered_map>

class GraphRenderer {
    private:
        const std::unordered_map<int, Node>& nodes;
        ImVec2 offset{0.0f, 0.0f};
        float zoom = 1.0f;
        ImVec2 toScreen(float x, float y) const;
        void handleInput();
    public:
        void draw();
        GraphRenderer(const std::unordered_map<int, Node>& nodes);
};