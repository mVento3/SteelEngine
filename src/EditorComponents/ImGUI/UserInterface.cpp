#include "EditorComponents/ImGUI/UserInterface.h"

#include "imgui/imgui.h"

namespace SteelEngine { namespace EditorComponents { namespace ImGUI {

    UserInterface::UserInterface()
    {

    }

    UserInterface::~UserInterface()
    {

    }

    void UserInterface::Init()
    {

    }

    void UserInterface::Draw()
    {

    }

    void UserInterface::OnRecompile(HotReloader::IRuntimeObject* oldObject)
    {
        ImGui::SetCurrentContext((ImGuiContext*)m_Context);
    }

}}}