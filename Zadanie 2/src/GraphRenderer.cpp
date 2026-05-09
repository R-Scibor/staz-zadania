#include "GraphRenderer.hpp"
#include "imgui.h"

GraphRenderer::GraphRenderer(const std::unordered_map<int, Node>& nodes) : nodes(nodes) {
}

void GraphRenderer::draw() {
    for (const auto& [id, node] : nodes) {
        ImVec2 pos(node.x, node.y);
        for(int childId : node.children){
            ImVec2 childPos(nodes.at(childId).x, nodes.at(childId).y);
            ImGui::GetForegroundDrawList ()->AddLine(ImVec2(pos.x + NODE_WIDTH, pos.y + NODE_HEIGHT/2), ImVec2(childPos.x, childPos.y + NODE_HEIGHT/2), IM_COL32(255, 255, 255, 255), 2.0f);
        }
    }
    for (const auto& [id, node] : nodes) {
        ImVec2 pos(node.x, node.y);
        ImVec2 textSize = ImGui::CalcTextSize(node.name.c_str());
        ImGui::GetForegroundDrawList ()->AddRectFilled(pos, ImVec2(pos.x + NODE_WIDTH, pos.y + NODE_HEIGHT), IM_COL32(100, 100, 255, 255));
        ImGui::GetForegroundDrawList ()->AddText(ImVec2(pos.x + NODE_WIDTH / 2 - textSize.x / 2, pos.y + NODE_HEIGHT / 2 - textSize.y / 2), IM_COL32(255, 255, 255, 255), node.name.c_str());
    }
}