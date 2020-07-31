#include "Editor/ImGUI/EditorWindows/DeferredBuffersViewWindow.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

namespace SteelEngine { namespace Editor { namespace ImGUI {

    DeferredBuffersViewWindow::DeferredBuffersViewWindow()
    {
        Event::GlobalEvent::Add<AddFramebuffer>(this);
    }

    DeferredBuffersViewWindow::~DeferredBuffersViewWindow()
    {

    }

    void DeferredBuffersViewWindow::Init()
    {
        
    }

    void DeferredBuffersViewWindow::Draw()
    {
        if(!GImGui)
        {
            return;
        }

        for(uint32_t id : m_Textures)
        {
            ImGui::Image((void*)id, ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
        }
    }

    void DeferredBuffersViewWindow::OnRecompile(HotReloader::IRuntimeObject* oldObject)
    {
        UserInterface::OnRecompile(oldObject);
    }

    void DeferredBuffersViewWindow::operator()(const AddFramebuffer& event)
    {
        // m_Test = event.m_ID;
        m_Textures.push_back(event.m_ID);
    }

}}}