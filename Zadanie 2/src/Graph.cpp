#include "Graph.hpp"
#include <vector>
#include <iostream>


bool Graph::updateLayerSizes(int prevLayer, int childId) {
    int nextLayer = prevLayer + 1;
    bool deepened = false;
    if (nextLayer >= layerSizes.size()) {
        layerSizes.resize(nextLayer + 1, 0);
    }
    if(sortedNodes.at(childId).layer < nextLayer){
        if(sortedNodes.at(childId).layer != -1){
            layerSizes[sortedNodes.at(childId).layer]--;
            deepened = true;
        }

        sortedNodes[childId].layer = nextLayer;
        layerSizes[nextLayer]++;
    }
    return deepened;
}

void Graph::calculateNodePositions() {
    std::vector<std::vector<int>> nodesByLayer(layerSizes.size());
    for (auto& [id, node] : sortedNodes) {
        nodesByLayer[node.layer].push_back(id);
    }

    for (int i = 0; i < layerSizes.size(); i++) {
        int startY = (CANVAS_HEIGHT - layerSizes[i] * YDISTANCE) / 2;

        std::vector<std::pair<int, float>> currentLayerAvg;
        for (int id : nodesByLayer[i]) {
            Node& node = sortedNodes.at(id);
            node.x = XDISTANCE * (i + 1);

            float avg = 0.0f;
            if (!node.parents.empty()) {
                for (int parentId : node.parents) {
                    avg += sortedNodes.at(parentId).y;
                }
                avg /= node.parents.size();
            }
            currentLayerAvg.push_back({id, avg});
        }

        std::sort(currentLayerAvg.begin(), currentLayerAvg.end(),
                  [](const auto& a, const auto& b) { return a.second < b.second; });

        for (int j = 0; j < currentLayerAvg.size(); j++) {
            sortedNodes.at(currentLayerAvg[j].first).y = startY + j * YDISTANCE;
        }
    }
}

// We assume that the input graph does not contain cycles. If it does,
// the function will detect it and print a warning, but the behavior is undefined.
std::unordered_map<int, Node> Graph::assignLayers(const std::unordered_map<int, Node>& nodeMap) {
    std::vector<int> NodeQueue;
    std::unordered_map<int, int> popCount;

    // Find roots
    for (const auto& [id, node]: nodeMap) {
        if (node.parents.empty()) {
            NodeQueue.push_back(id);
            sortedNodes[id] = node;
            updateLayerSizes(-1, id);
        }
    }

    while (!NodeQueue.empty()) {
        int currentId = NodeQueue.back();
        NodeQueue.pop_back();

        if (++popCount[currentId] > nodeMap.size()) {
            std::cerr << "Warning: cycle detected, aborting layout.\n";
            break;
        }

        int currentLayer = sortedNodes.at(currentId).layer;
        const Node& currentNode = nodeMap.at(currentId);

        for (int childId : currentNode.children) {
            auto [it, isNew] = sortedNodes.insert({childId, nodeMap.at(childId)});
            if (updateLayerSizes(currentLayer, childId) || isNew) {
                NodeQueue.push_back(childId);
            }
        }
    }

    return sortedNodes;
}