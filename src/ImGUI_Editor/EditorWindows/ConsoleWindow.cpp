#include "ImGUI_Editor/EditorWindows/ConsoleWindow.h"

namespace SteelEngine {

    ConsoleWindow::ConsoleWindow()
    {

    }

    ConsoleWindow::~ConsoleWindow()
    {

    }

    void ConsoleWindow::Draw()
    {
        ImGui::Text("XXXXXX");
    }

    void ConsoleWindow::operator()(const RecompiledEvent& event)
    {
        ImGui::SetCurrentContext((ImGuiContext*)m_Context);
    }

}