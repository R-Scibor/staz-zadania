#pragma once
#include "Graph.hpp"
#include "NodeStruct.hpp"
#include "Config.hpp"
#include <unordered_map>

class GraphRenderer {
    private:
        std::unordered_map<int, Node>& nodes;
    public:
        void draw();
        GraphRenderer(std::unordered_map<int, Node>& nodes);
};