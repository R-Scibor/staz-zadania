cmake_minimum_required(VERSION 3.15)
project(GraphRenderer)

set(CMAKE_CXX_STANDARD 20)

find_package(GTest REQUIRED)

add_executable(graph_renderer src/main.cpp src/GraphRenderer.cpp)

add_executable(graph_renderer_tests tests/GraphRendererTest.cpp src/GraphRenderer.cpp)
target_link_libraries(graph_renderer_tests GTest::gtest_main)
target_include_directories(graph_renderer_tests PRIVATE src)
enable_testing()
add_test(NAME GraphRendererTests COMMAND graph_renderer_tests)

