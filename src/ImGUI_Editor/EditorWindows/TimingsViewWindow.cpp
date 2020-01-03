#include "ImGUI_Editor/EditorWindows/TimingsViewWindow.h"

#include "RuntimeReflection/Reflection.h"

#include "Core/Core.h"

#include "imgui/imgui.h"

namespace SteelEngine {

    TimingsViewWindow::TimingsViewWindow()
    {
        m_Headers =
        {
            { "LOL", 50.f },
            { "LOL2", 50.f }
        };
    }

    TimingsViewWindow::~TimingsViewWindow()
    {

    }

    void TimingsViewWindow::Init()
    {
        m_Manager = Reflection::GetType("SteelEngine::Core")->GetMetaData(Core::GlobalSystems::PROFILER)->Convert<Profiler::IManager**>();
        m_Type = Reflection::GetType(*m_Manager);
    }

    void TimingsViewWindow::Draw()
    {
        std::vector<Profiler::Manager::TimeData>* timings = m_Type->Invoke("GetTime", *m_Manager).Convert<std::vector<Profiler::Manager::TimeData>*>();

        ImGui::Columns(3, "mycolumns");
        ImGui::Separator();
        ImGui::Text("ID"); ImGui::NextColumn();
        ImGui::Text("Name"); ImGui::NextColumn();
        ImGui::Text("Time"); ImGui::NextColumn();
        ImGui::Separator();

        static int selected = -1;

        for(Type::uint32 i = 0; i < timings->size(); i++)
        {
            const Profiler::Manager::TimeData& time = timings->at(i);
            char label[32];

            sprintf(label, "%d", i);

            if(ImGui::Selectable(label, selected == i, ImGuiSelectableFlags_SpanAllColumns))
                selected = i;

            ImGui::NextColumn();
            ImGui::Text(time.m_Name.c_str()); ImGui::NextColumn();
            ImGui::Text("%f ms", time.m_Time); ImGui::NextColumn();
        }

        ImGui::Columns(1);
        ImGui::Separator();
    }

}