#pragma once
#include <vector>
#include <string>

struct Node {
    int id;
    std::string name;
    float x, y;
    int layer = -1;
    std::vector<int> children;
    std::vector<int> parents;
};