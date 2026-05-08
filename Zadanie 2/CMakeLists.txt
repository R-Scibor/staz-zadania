cmake_minimum_required(VERSION 3.15)
project(GraphRenderer)

set(CMAKE_CXX_STANDARD 20)

find_package(GTest REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(glfw3 REQUIRED)
find_package(imgui REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(graph_renderer
    src/main.cpp
    src/Graph.cpp
    src/JSONParser.cpp
    src/GraphRenderer.cpp
    src/backends/imgui_impl_glfw.cpp
    src/backends/imgui_impl_opengl3.cpp
)
target_link_libraries(graph_renderer
    imgui::imgui
    glfw
    OpenGL::GL
    nlohmann_json::nlohmann_json
)
target_include_directories(graph_renderer PRIVATE src src/backends)

add_executable(json_parser_tests tests/JSONParserTest.cpp src/JSONParser.cpp)
target_link_libraries(json_parser_tests GTest::gtest_main nlohmann_json::nlohmann_json)
target_include_directories(json_parser_tests PRIVATE src)
target_compile_definitions(json_parser_tests PRIVATE TEST_DATA_DIR="${CMAKE_SOURCE_DIR}")

add_executable(graph_tests tests/GraphTest.cpp src/Graph.cpp)
target_link_libraries(graph_tests GTest::gtest_main nlohmann_json::nlohmann_json)
target_include_directories(graph_tests PRIVATE src)

enable_testing()

add_test(NAME JSONParserTests COMMAND json_parser_tests)
add_test(NAME GraphTests COMMAND graph_tests)
