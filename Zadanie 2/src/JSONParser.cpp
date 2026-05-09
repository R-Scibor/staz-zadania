#include "JSONParser.hpp"
#include <algorithm>
#include <sstream>
#include <vector>

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
        nodeMap.at(from).children.push_back(to);
        nodeMap.at(to).parents.push_back(from);            
    }

    return nodeMap;
}

std::string JSONParser::serializeJSON(const std::unordered_map<int, Node>& nodes) {
    std::vector<int> ids;
    ids.reserve(nodes.size());
    for (const auto& [id, _] : nodes) {
        ids.push_back(id);
    }
    std::sort(ids.begin(), ids.end());

    std::vector<std::pair<int, int>> edges;
    for (int id : ids) {
        for (int child : nodes.at(id).children) {
            edges.push_back({id, child});
        }
    }

    std::ostringstream out;
    out << "{\n\"nodes\": [\n";
    for (size_t i = 0; i < ids.size(); i++) {
        const Node& node = nodes.at(ids[i]);
        out << "{ \"id\": " << node.id
            << ", \"name\": " << nlohmann::json(node.name).dump()
            << ", \"x\": " << node.x
            << ", \"y\": " << node.y
            << " }";
        if (i + 1 < ids.size()) out << ",";
        out << "\n";
    }
    out << "],\n\"edges\": [\n";
    for (size_t i = 0; i < edges.size(); i++) {
        out << "{ \"from\": " << edges[i].first
            << ", \"to\": " << edges[i].second
            << " }";
        if (i + 1 < edges.size()) out << ",";
        out << "\n";
    }
    out << "]\n}";
    return out.str();
}