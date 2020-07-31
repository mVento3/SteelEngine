#include "Editor/ImGUI/EditorWindows/TimingsViewWindow.h"

#include "RuntimeReflection/Reflection.h"

#include "Core/Core.h"

#include "imgui/imgui.h"

#include "Profiler/ScopeTimer.h"

#include "Logger/Logger.h"

namespace SteelEngine {

    TimingsViewWindow::TimingsViewWindow()
    {

    }

    TimingsViewWindow::~TimingsViewWindow()
    {

    }

    void TimingsViewWindow::Init()
    {
        m_Manager = Reflection::GetType("SteelEngine::Core")->GetMetaData(Core::GlobalSystems::PROFILER)->Convert<Profiler::IManager**>();
        m_Type = Reflection::GetType(*m_Manager);
        m_GetTimeMethod = m_Type->GetMethod("GetTime");
        m_Timings = m_Type->Invoke(m_GetTimeMethod, *m_Manager).Convert<std::vector<Profiler::Manager::TimeData>*>();
    }

    void TimingsViewWindow::Draw()
    {
        {
            SE_PROFILE_SCOPE("TimingsViewWindow::Draw");

            ImGui::Columns(3, "mycolumns");
            ImGui::Separator();
            ImGui::SetColumnWidth(0, ImGui::CalcTextSize(" ID ").x); ImGui::Text("ID"); ImGui::NextColumn();
            ImGui::Text("Name"); ImGui::NextColumn();
            ImGui::Text("Time"); ImGui::NextColumn();
            ImGui::Separator();

            static int selected = -1;

            for(Type::uint32 i = 0; i < m_Timings->size(); i++)
            {
                const Profiler::Manager::TimeData& time = m_Timings->at(i);
                char label[32];

                sprintf(label, "%d", i);

                if(ImGui::Selectable(label, selected == i, ImGuiSelectableFlags_SpanAllColumns))
                {
                    selected = i;
                }

                ImGui::NextColumn();
                ImGui::Text(time.m_Name.c_str()); ImGui::NextColumn();

                if(ImGui::IsItemHovered())
                {
                    ImGui::BeginTooltip();
                    {
                        ImGui::Text(time.m_Name.c_str());
                    }
                    ImGui::EndTooltip();
                }

                if(ImGui::IsMouseDoubleClicked(0) && selected == i)
                {
                    system(std::string("code -g " + time.m_File.string() + ":" + std::to_string(time.m_Line)).c_str());
                }

                ImGui::Text("%f ms", time.m_Time); ImGui::NextColumn();
            }

            ImGui::Columns(1);
            ImGui::Separator();
        }
    }

}