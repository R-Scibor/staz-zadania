#pragma once
#include "NodeStruct.hpp"
#include "JSONParser.hpp"
#include "config.hpp"
#include <unordered_map>
#include <vector>

class Graph {
    private:
    std::unordered_map<int, Node> sortedNodes;
    std::vector<int> layerSizes;
    void updateLayerSizes(int prevLayer, int childId);

    public:
        std::unordered_map<int, Node> assignLayers(const std::unordered_map<int, Node>& nodeMap);
        void calculateNodePositions();

        std::unordered_map<int, Node> getSortedNodes() const {
            return sortedNodes;
        }
        std::vector<int> getLayerSizes() const {
            return layerSizes;
        }

};