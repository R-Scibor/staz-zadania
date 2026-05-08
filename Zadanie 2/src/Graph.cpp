#include "Graph.hpp"
#include <vector>


void Graph::updateLayerSizes(int prevLayer, int childId) {
    int nextLayer = prevLayer + 1;
    if (nextLayer >= layerSizes.size()) {
        layerSizes.resize(nextLayer + 1, 0);
    }
    if(sortedNodes[childId].layer < nextLayer){
        if(sortedNodes[childId].layer != -1)
            layerSizes[sortedNodes[childId].layer]--;

        sortedNodes[childId].layer = nextLayer;
        layerSizes[nextLayer]++;
    }
}

void Graph::calculateNodePositions() {
    std::vector<std::pair<int, float>> currentLayerAvg;

    for(int i=0; i<layerSizes.size(); i++){
        int startY = (height - layerSizes[i] * YDISTANCE) / 2;
        for(auto& [id, node]: sortedNodes){
            if(node.layer == i){
                node.x = XDISTANCE * (i+1);
                if (i == 0) {
                    currentLayerAvg.push_back({id, 0});
                }
                else {
                    int avg = 0;
                    for (auto parentId : node.parents) {
                        avg += sortedNodes.at(parentId).y;
                    }
                    avg /= node.parents.size();
                    currentLayerAvg.push_back({id, avg});
                }
            }
        }
        std::sort(currentLayerAvg.begin(), currentLayerAvg.end(), [](const auto& a, const auto& b){ return a.second < b.second; });
        for(int j=0; j<currentLayerAvg.size(); j++){
            sortedNodes[currentLayerAvg[j].first].y = startY + j * YDISTANCE;
        }
        currentLayerAvg.clear();
    }
}


std::unordered_map<int, Node> Graph::assignLayers(const std::unordered_map<int, Node>& nodeMap) {
    std::vector<int> NodeQueue;

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
        const Node& currentNode = sortedNodes[currentId];
        int nextLayer = currentNode.layer + 1;

        for (int childId : currentNode.children) {
            if (sortedNodes.find(childId) == sortedNodes.end()) {
                sortedNodes[childId] = nodeMap.at(childId);
                NodeQueue.push_back(childId);
            }
            updateLayerSizes(currentNode.layer, childId);
        }
    }

    return sortedNodes;
}