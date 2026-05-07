#include "JSONParser.hpp"

std::unordered_map<int, Node> JSONParser::parseJSON(const std::string& jsonString) {
    std::unordered_map<int, Node> nodeMap;
    auto json = nlohmann::json::parse(jsonString);

    for (const auto& item : json["nodes"]) {
        Node node;
        node.id = item["id"];
        node.name = item["name"];
        node.x = item["x"];
        node.y = item["y"];
        nodeMap[node.id] = node;
    }

    for (const auto& edge : json["edges"]) {
        int from = edge["from"];
        int to = edge["to"];
        nodeMap[from].children.push_back(to);
        nodeMap[to].parents.push_back(from);                //todo: check if from and to exist in nodeMap, otherwise log error
    }

    return nodeMap;
}