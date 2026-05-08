#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include "JSONParser.hpp"

static std::unordered_map<int, Node> parseFile(const std::string& filename) {
    std::ifstream file(std::string(TEST_DATA_DIR) + "/" + filename);
    std::stringstream ss;
    ss << file.rdbuf();
    return JSONParser().parseJSON(ss.str());
}

TEST(JSONParserTest, LoadsGraphJsonFromDisk) {
    auto nodes = parseFile("graph.json");

    ASSERT_EQ(nodes.size(), 4);
    EXPECT_EQ(nodes[1].name, "Event BeginPlay");
    EXPECT_EQ(nodes[2].name, "Branch");
    EXPECT_EQ(nodes[3].name, "Play Sound (True)");
    EXPECT_EQ(nodes[4].name, "Spawn Actor (False)");
}

TEST(JSONParserTest, ParsesNodeFields) {
    std::string json = R"({
        "nodes": [{ "id": 1, "name": "Event BeginPlay", "x": 10, "y": 20 }],
        "edges": []
    })";
    auto nodes = JSONParser().parseJSON(json);

    ASSERT_EQ(nodes.size(), 1);
    EXPECT_EQ(nodes[1].id, 1);
    EXPECT_EQ(nodes[1].name, "Event BeginPlay");
    EXPECT_FLOAT_EQ(nodes[1].x, 10.0f);
    EXPECT_FLOAT_EQ(nodes[1].y, 20.0f);
}

TEST(JSONParserTest, EmptyGraph) {
    std::string json = R"({ "nodes": [], "edges": [] })";
    auto nodes = JSONParser().parseJSON(json);

    EXPECT_TRUE(nodes.empty());
}
