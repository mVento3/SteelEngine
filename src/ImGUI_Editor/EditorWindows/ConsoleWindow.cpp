#include "ImGUI_Editor/EditorWindows/ConsoleWindow.h"

#include "imgui/imgui.h"

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

    void ConsoleWindow::OnRecompile(HotReloader::IRuntimeObject* oldObject)
    {
        EditorComponents::ImGUI::UserInterface::OnRecompile(oldObject);
        ImGui::SetCurrentContext((ImGuiContext*)m_Context);
    }

}