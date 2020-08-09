#include "Editor/ImGUI/EditorWindows/StartMenuWindow.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include "RuntimeReflection/Reflection.h"

#include "VirtualProject/LoadProjectEvent.h"
#include "VirtualProject/CreateNewProjectEvent.h"

#include "Networking/NetworkManager.h"

#include "Utils/Json.h"

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

            // TODO: We should delete the thread when complete

            // if((*m_VirtualProject)->IsProjectLoadedSuccessful() == SE_TRUE)
            // {
            //     Event::GlobalEvent::Broadcast(ChangeSceneEvent{ SceneType::EDITOR_SCENE });
            // }
        });
    }

    void StartMenuWindow::CreateNewProject()
    {
        ImGui::OpenPopup("##createNewProject");
    }

    StartMenuWindow::StartMenuWindow()
    {
        m_Size = 32;
        m_NewProjectName = new char[m_Size];

        for(Type::uint32 i = 0; i < m_Size; i++)
        {
            m_NewProjectName[i] = '\0';
        }

        m_VirtualProject = 0;
        m_ProjectInitialized = false;
    }

    StartMenuWindow::~StartMenuWindow()
    {

    }

    void StartMenuWindow::Init()
    {
        m_ProjectsPath = Reflection::GetType("SteelEngine::Core")->GetMetaData(Reflection::ReflectionAttribute::PROJECTS_PATH)->Convert<std::string>();

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

    // TODO: That is not supposed to be here lol
        if(m_VirtualProject && (*m_VirtualProject) && !m_ProjectInitialized && (*m_VirtualProject)->IsProjectLoadedSuccessful() == SE_TRUE)
        {
            m_ProjectInitialized = true;

            (*(*m_VirtualProject)->GetVisualizer())->OnProjectLoad();
        }

        if(ImGui::BeginPopup("##createNewProject"))
        {
            ImGui::InputText("Project name", m_NewProjectName, m_Size);

            if(ImGui::Button("Create"))
            {
                ProjectStructure proj(m_NewProjectName);
                CreateNewProjectEvent event(proj, m_ProjectsPath);

                Event::GlobalEvent::Broadcast_(&event);

                m_VirtualProject = event.m_Project;

                LoadProject(m_ProjectsPath / proj.m_DirectoryName);

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

    void StartMenuWindow::OnRecompile(HotReloader::IRuntimeObject* oldObject)
    {
        UserInterface::OnRecompile(oldObject);
    }

}}}