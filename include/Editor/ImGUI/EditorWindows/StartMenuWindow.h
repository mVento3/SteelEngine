#pragma once

#include "RuntimeReflection/Macro.h"

#include "Editor/ImGUI/ReflectionAttribs.h"
#include "Editor/ImGUI/Window.h"
#include "Editor/ImGUI/ImGUI_Editor.h"
#include "Editor/ImGUI/SelectableProject.h"
#include "Editor/ImGUI/WindowType.h"

#include "VirtualProject/IVirtualProject.h"

#include "thread"

#include "EditorComponents/ImGUI/UserInterface.h"

#include "Editor/ImGUI/EditorWindows/StartMenuWindow.Generated.h"

namespace SteelEngine { namespace Editor { namespace ImGUI {

    SE_CLASS(
        SteelEngine::Editor::ReflectionAttributes::EDITOR_WINDOW,
        SteelEngine::Editor::ReflectionAttributes::WINDOW_TYPE = SteelEngine::Editor::WindowType::STATIC,
        SteelEngine::Reflection::ReflectionAttribute::RUNTIME_SERIALIZE,
        SteelEngine::Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS,
        SteelEngine::EditorComponents::ImGUI::UserInterface::Attributes::SEPARATE_WINDOW,
        Reflection::ReflectionAttribute::HOT_RELOAD
    )
    class StartMenuWindow :
        public EditorComponents::ImGUI::UserInterface,
        public HotReloader::IRuntimeObject
    {
        GENERATED_BODY
    private:
        std::string m_SelectedProject;
        IVirtualProject** m_VirtualProject;
        std::filesystem::path m_ProjectsPath;

        std::thread* m_LoadProjectThread;
        bool m_ProjectInitialized;

        std::vector<SelectableProject> m_SelectableProjects;

        char* m_NewProjectName;
        size_t m_Size;

        void LoadProject(const std::filesystem::path& path);
        void CreateNewProject();

    public:
        StartMenuWindow();
        ~StartMenuWindow();

        void Init() override;
        void Draw() override;

        void OnRecompile(HotReloader::IRuntimeObject* oldObject) override;
    };

}}}