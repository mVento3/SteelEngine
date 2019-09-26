#include "ImGUI_Editor/EditorWindows/StartMenuWindow.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include "RuntimeReflection/Reflection.h"

#include "VirtualProject/LoadProjectEvent.h"

#include "Networking/NetworkManager.h"

namespace SteelEngine { namespace Editor { namespace ImGUI {

    void StartMenuWindow::LoadProject(const std::filesystem::path& path)
    {
        // Here we should load or the .json file with info or .py file
        // Or just simply load .dll with compiled scripts, reflection, etc.

        m_LoadProjectThread = new std::thread([this, path]()
        {
            LoadProjectEvent event;

            event.m_Path = path;

            Event::GlobalEvent::Broadcast_(&event);

            m_VirtualProject = event.m_VirtualProject;

            while((*m_VirtualProject)->IsProjectLoaded() == SE_FALSE)
            {

            }

            Event::GlobalEvent::Broadcast(ChangeSceneEvent{ SceneType::EDITOR_SCENE });
        });
    }

    void StartMenuWindow::CreateNewProject()
    {
        ImGui::OpenPopup("##createNewProject");
    }

    StartMenuWindow::StartMenuWindow()
    {

    }

    StartMenuWindow::~StartMenuWindow()
    {

    }

    void StartMenuWindow::Init()
    {
        m_ProjectsPath = Reflection::GetType("SteelEngine::Core")->GetMetaData(ReflectionAttribute::PROJECTS_PATH)->Convert<const char*>();

        printf("Projects path: %s\n", m_ProjectsPath.string().c_str());

    // Gather for projects
        for(auto& path : std::filesystem::directory_iterator(m_ProjectsPath))
        {
            if(std::filesystem::is_directory(path.path()))
            {
                ProjectInfo p;
                SelectableProject sp;

                p.m_Path = path.path().string();
                p.m_Name = path.path().filename().string();

                sp.m_ProjectInfo = p;

                m_SelectableProjects.push_back(sp);
            }
        }
    }

    void StartMenuWindow::Draw()
    {
        if(!GImGui)
        {
            return;
        }

        if(ImGui::BeginPopup("##createNewProject"))
        {
            ImGui::InputText("Project name", m_NewProjectName, IM_ARRAYSIZE(m_NewProjectName));

            if(ImGui::Button("Create"))
            {
                ProjectStructure proj;

                proj.m_DirectoryName = m_NewProjectName;
                proj.m_Other.push_back(ProjectStructure{ "bin" });
                proj.m_Other.push_back(ProjectStructure{ "build", { ProjectStructure{ "Windows" }, ProjectStructure{ "Linux" } } });
                proj.m_Other.push_back(ProjectStructure{ "src" });
                proj.m_Other.push_back(ProjectStructure{ "include" });

                (*m_VirtualProject)->CreateProject(m_ProjectsPath.string(), proj);
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        if(ImGui::BeginCombo("Projects", &m_SelectedProject[0]))
        {
            for(int n = 0; n < m_SelectableProjects.size(); n++)
            {
                SelectableProject& sp = m_SelectableProjects[n];

                bool is_selected = (m_SelectedProject == sp.m_ProjectInfo.m_Name);

                if(ImGui::Selectable(sp.m_ProjectInfo.m_Name.c_str(), is_selected))
                {
                    m_SelectedProject = sp.m_ProjectInfo.m_Name;
                }

                if(is_selected)
                {
                    ImGui::SetItemDefaultFocus();
                }

                if(ImGui::IsItemHovered())
                {
                    ImGui::BeginTooltip();
                    {
                        ImGui::Text("Some text, info about project");
                    }
                    ImGui::EndTooltip();
                }
            }

            ImGui::EndCombo();
        }

        if(m_SelectedProject == "")
        {
            if(ImGui::Button("New project"))
            {
                CreateNewProject();
            }
        }
        else
        {
            if(ImGui::ButtonEx("New project", ImVec2(0, 0), ImGuiButtonFlags_::ImGuiButtonFlags_Disabled))
            {
                // None cuz disabled
            }
        }

        ImGui::SameLine();

        if(m_SelectedProject != "")
        {
            if(ImGui::Button("Load project"))
            {
                LoadProject(m_ProjectsPath / m_SelectedProject);
            }
        }
        else
        {
            if(ImGui::ButtonEx("Load project", ImVec2(0, 0), ImGuiButtonFlags_::ImGuiButtonFlags_Disabled))
            {
                // None cuz disabled
            }
        }
    }

    void StartMenuWindow::operator()(const RecompiledEvent& event)
    {
        ImGui::SetCurrentContext((ImGuiContext*)m_Context);
    }

}}}