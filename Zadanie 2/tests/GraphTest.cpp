#include <gtest/gtest.h>
#include "Graph.hpp"

static std::unordered_map<int, Node> makeChain() {
    // 1 -> 2 -> 3
    std::unordered_map<int, Node> m;
    m[1] = {1, "A", 0, 0, -1, {2}, {}};
    m[2] = {2, "B", 0, 0, -1, {3}, {1}};
    m[3] = {3, "C", 0, 0, -1, {},  {2}};
    return m;
}

static std::unordered_map<int, Node> makeDiamond() {
    // 1 -> 2, 1 -> 3, 2 -> 4, 3 -> 4
    std::unordered_map<int, Node> m;
    m[1] = {1, "A", 0, 0, -1, {2, 3}, {}};
    m[2] = {2, "B", 0, 0, -1, {4},    {1}};
    m[3] = {3, "C", 0, 0, -1, {4},    {1}};
    m[4] = {4, "D", 0, 0, -1, {},     {2, 3}};
    return m;
}

TEST(GraphTest, LinearChainLayers) {
    auto result = Graph().assignLayers(makeChain());

    EXPECT_EQ(result.at(1).layer, 0);
    EXPECT_EQ(result.at(2).layer, 1);
    EXPECT_EQ(result.at(3).layer, 2);
}

TEST(GraphTest, DiamondNodePlacedOnDeepestLayer) {
    auto result = Graph().assignLayers(makeDiamond());

    EXPECT_EQ(result.at(1).layer, 0);
    EXPECT_EQ(result.at(2).layer, 1);
    EXPECT_EQ(result.at(3).layer, 1);
    EXPECT_EQ(result.at(4).layer, 2);
}

TEST(GraphTest, LongPathWinsOverShortPath) {
    // 1 -> 3 -> 2 -> 4   (long path)
    // 1 -> 2             (shortcut)
    std::unordered_map<int, Node> m;
    m[1] = {1, "A", 0, 0, -1, {3, 2}, {}};
    m[2] = {2, "B", 0, 0, -1, {4},    {1, 3}};
    m[3] = {3, "C", 0, 0, -1, {2},    {1}};
    m[4] = {4, "D", 0, 0, -1, {},     {2}};

    auto result = Graph().assignLayers(m);
    EXPECT_EQ(result.at(2).layer, 2);
    EXPECT_EQ(result.at(4).layer, 3);
}

TEST(GraphPositionsTest, XIncreasesWithLayer) {
    Graph g;
    g.assignLayers(makeChain());
    g.calculateNodePositions();
    auto result = g.getSortedNodes();

    EXPECT_LT(result.at(1).x, result.at(2).x);
    EXPECT_LT(result.at(2).x, result.at(3).x);
}

TEST(GraphPositionsTest, NodesOnSameLayerNotOverlapping) {
    // nodes 1 and 2 both on layer 0
    std::unordered_map<int, Node> m;
    m[1] = {1, "A", 0, 0, -1, {3}, {}};
    m[2] = {2, "B", 0, 0, -1, {3}, {}};
    m[3] = {3, "C", 0, 0, -1, {}, {1, 2}};

    Graph g;
    g.assignLayers(m);
    g.calculateNodePositions();
    auto result = g.getSortedNodes();

    EXPECT_NE(result.at(1).y, result.at(2).y);
}
