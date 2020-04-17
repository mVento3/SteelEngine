#include "EditorComponents/ImGUI/UserInterface.h"

#include "imgui/imgui.h"

namespace SteelEngine { namespace EditorComponents { namespace ImGUI {

    UserInterface::UserInterface()
    {
        m_Draw = true;
        m_IsWindowOpen = true;
    }

    UserInterface::~UserInterface()
    {

    }

    void UserInterface::Init()
    {

    }

    void UserInterface::DrawUser()
    {
        if(m_Draw)
        {
            Draw();
        }
    }

    void UserInterface::OnRecompile(HotReloader::IRuntimeObject* oldObject)
    {
        ImGui::SetCurrentContext((ImGuiContext*)m_Context);
    }

    // void UserInterface::OnSwap(HotReloader::SwapStage stage, HotReloader::ObjectAge age)
    // {
    //     if(stage == HotReloader::SwapStage::BEFORE && age == HotReloader::ObjectAge::OLD)
    //     {
    //         m_Draw = false;
    //     }
    //     else if(stage == HotReloader::SwapStage::AFTER && age == HotReloader::ObjectAge::YOUNG)
    //     {
    //         ImGui::SetCurrentContext((ImGuiContext*)m_Context);
    //         m_Draw = true;
    //     }
    // }

}}}