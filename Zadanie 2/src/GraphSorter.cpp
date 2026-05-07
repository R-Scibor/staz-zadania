#include "GraphSorter.hpp"
#include <vector>


std::unordered_map<int, Node> GraphSorter::topologicalSort(const std::unordered_map<int, Node>& nodeMap) {
    std::unordered_map<int, Node> sortedNodes;
    std::vector<int> NodeQueue;

    std::vector<int> layerSizes;

    // Find roots
    for (const auto& pair : nodeMap) {
        if (pair.second.parents.empty()) {
            NodeQueue.push_back(pair.first);
            sortedNodes[pair.first] = pair.second;
            sortedNodes[pair.first].layer = 0; 
        }
    }

    //TODO: Fold the loops into one queue processing  ???

    while (!NodeQueue.empty()) {
        int currentId = NodeQueue.back();
        NodeQueue.pop_back();
        const Node& currentNode = sortedNodes[currentId];

        for (int childId : currentNode.children) {
            if (sortedNodes.find(childId) == sortedNodes.end()) {
                sortedNodes[childId] = nodeMap.at(childId);
                NodeQueue.push_back(childId);
            }
            if(sortedNodes[childId].layer < currentNode.layer + 1)
                sortedNodes[childId].layer = currentNode.layer + 1; 
        }
    }

    for (const auto& pair : sortedNodes) {
        int layer = pair.second.layer;
        if (layer >= layerSizes.size()) {
            layerSizes.resize(layer + 1, 0);
        }
        layerSizes[layer]++;
    }

    std::vector<int> layercounter(layerSizes.size(), 0);
    int height = 600; //todo temp canvas height
    int width = 800; //todo temp canvas width
    for (auto& pair : sortedNodes) {

        int layer = pair.second.layer;
        pair.second.x = XDISTANCE * (layer+1);
        if (layer == 0) {

            int startY = (height - layerSizes[layer] * YDISTANCE) / 2;

            pair.second.y = startY + layercounter[layer] * YDISTANCE;

            layercounter[layer]++;
            continue;
        }
        else {
            int avg = 0;
            for (auto parentId : pair.second.parents) {
                avg += sortedNodes.at(parentId).y;
            }
            avg /= pair.second.parents.size();

            pair.second.y = avg;

        }
        layercounter[layer]++;
    }



    return sortedNodes;
}