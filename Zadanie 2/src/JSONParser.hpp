#pragma once
#include "nlohmann/json.hpp"
#include "NodeStruct.hpp"
#include <string>
#include <unordered_map>
#include <vector>

class JSONParser {
public:
    std::unordered_map<int, Node> parseJSON(const std::string& jsonString);
};