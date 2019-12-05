#include "ImGUI_Editor/EditorWindows/SystemsMonitorWindow.h"

#include "InformationTracker/IInformationTracker.h"

#include "Event/GlobalEventTracker.h"

#include "Core/Core.h"

#include "imgui/imgui.h"

namespace SteelEngine { namespace Editor { namespace ImGUI {

    SystemsMonitorWindow::SystemsMonitorWindow()
    {
        m_CurrentItem = 0;
        m_CurrentReflectionItem = 0;
    }

    SystemsMonitorWindow::~SystemsMonitorWindow()
    {

    }
    
    void SystemsMonitorWindow::Init()
    {
        m_Info = Reflection::GetType("SteelEngine::Core")->GetMetaData(ReflectionAttribute::SYSTEMS_INFORMATION_TRACKER);

        if(!m_Database)
        {
            m_Database = (RuntimeDatabase*)ModuleManager::GetModule("RuntimeDatabase");
        }

        std::vector<IReflectionData*> types = Reflection::GetTypes();

        for(Type::uint32 i = 0; i < types.size(); i++)
        {
            reflectionRes.push_back(types[i]->GetTypeName());
        }
    }

    void SystemsMonitorWindow::Draw()
    {
        IDeltaTime** delta = Reflection::GetType("SteelEngine::Core")->GetMetaData(Core::GlobalSystems::DELTA_TIME)->Convert<IDeltaTime**>();

        ImGui::Text("%f ms", (*delta)->GetDeltaTime() * 1000.f);
        ImGui::Text("%u fps", (*delta)->GetUPS());
        ImGui::Text("Some info about engine systems...");

        if(ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            {
                ImGui::Text("Systems like reflection, events, etc.");
            }
            ImGui::EndTooltip();
        }

        ISystemTracker* tracker = m_Info->Convert<IInformationTracker*>()->Get(typeid(Event::GlobalEvent).hash_code());

        if(tracker)
        {
            GlobalEventTracker* tracker_ = (GlobalEventTracker*)tracker;

            std::vector<GlobalEventTracker::Info> info = tracker_->GetAll();
            std::vector<const char*> res;

            for(Type::uint32 i = 0; i < info.size(); i++)
            {
                res.push_back(info[i].m_ObjectName.c_str());
            }

            ImGui::ListBox("Global events", &m_CurrentItem, res.data(), res.size());
            ImGui::Text("Count of global events: %i", info.size());
            ImGui::Text("Additional info");
            ImGui::Text("%s", info[m_CurrentItem].m_EventType.c_str());
        }

        ImGui::Text("Loaded modules");

        std::vector<const char*> rees;

        for(Type::uint32 i = 0; i < reflectionRes.size(); i++)
        {
            rees.push_back(reflectionRes[i].c_str());
        }

        ImGui::ListBox("Reflection", &m_CurrentReflectionItem, rees.data(), rees.size());
    }

    void SystemsMonitorWindow::OnRecompile(HotReloader::IRuntimeObject* oldObject)
    {
        EditorComponents::ImGUI::UserInterface::OnRecompile(oldObject);
        ImGui::SetCurrentContext((ImGuiContext*)m_Context);
    }

}}}