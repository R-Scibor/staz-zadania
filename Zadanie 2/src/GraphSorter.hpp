#include "NodeStruct.hpp"
#include "JSONParser.hpp"
#include <unordered_map>

class GraphSorter {
    private:
    const int XDISTANCE = 100;
    const int YDISTANCE = 100;

    public:
        std::unordered_map<int, Node> topologicalSort(const std::unordered_map<int, Node>& nodeMap);
};