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

    void ConsoleWindow::OnRecompile(HotReload::IRuntimeObject* oldObject)
    {
        Window::OnRecompile(oldObject);
        ImGui::SetCurrentContext((ImGuiContext*)m_Context);
    }

}