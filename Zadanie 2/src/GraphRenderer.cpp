#include "GraphRenderer.hpp"
#include "imgui.h"

GraphRenderer::GraphRenderer(const std::unordered_map<int, Node>& nodes) : nodes(nodes) {
}

ImVec2 GraphRenderer::toScreen(float x, float y) const {
    return ImVec2(x * zoom + offset.x, y * zoom + offset.y);
}

void GraphRenderer::handleInput() {
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse) return;

    if (ImGui::IsMouseDragging(ImGuiMouseButton_Left, 0.0f)) {
        offset.x += io.MouseDelta.x;
        offset.y += io.MouseDelta.y;
    }

    if (io.MouseWheel != 0.0f) {
        ImVec2 mouse = io.MousePos;
        float worldX = (mouse.x - offset.x) / zoom;
        float worldY = (mouse.y - offset.y) / zoom;
        float factor = (io.MouseWheel > 0.0f) ? 1.1f : (1.0f / 1.1f);
        zoom *= factor;
        if (zoom < 0.1f) zoom = 0.1f;
        if (zoom > 5.0f) zoom = 5.0f;
        offset.x = mouse.x - worldX * zoom;
        offset.y = mouse.y - worldY * zoom;
    }
}

void GraphRenderer::draw() {
    handleInput();

    const float nw = NODE_WIDTH * zoom;
    const float nh = NODE_HEIGHT * zoom;
    ImDrawList* dl = ImGui::GetForegroundDrawList();

    for (const auto& [id, node] : nodes) {
        ImVec2 pos = toScreen(node.x, node.y);
        for(int childId : node.children){
            const Node& child = nodes.at(childId);
            ImVec2 childPos = toScreen(child.x, child.y);
            dl->AddLine(ImVec2(pos.x + nw, pos.y + nh/2), ImVec2(childPos.x, childPos.y + nh/2), IM_COL32(255, 255, 255, 255), 2.0f);
        }
    }

    ImFont* font = ImGui::GetFont();
    float fontSize = ImGui::GetFontSize() * zoom;
    for (const auto& [id, node] : nodes) {
        ImVec2 pos = toScreen(node.x, node.y);
        ImVec2 textSize = font->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, node.name.c_str());
        dl->AddRectFilled(pos, ImVec2(pos.x + nw, pos.y + nh), IM_COL32(100, 100, 255, 255));
        dl->AddText(font, fontSize, ImVec2(pos.x + nw / 2 - textSize.x / 2, pos.y + nh / 2 - textSize.y / 2), IM_COL32(255, 255, 255, 255), node.name.c_str());
    }
}
