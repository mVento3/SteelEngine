#pragma once

#include "RuntimeReflection/Macro.h"

#include "ImGUI_Editor/ReflectionAttribs.h"
#include "ImGUI_Editor/Window.h"
#include "ImGUI_Editor/ImGUI_Editor.h"
#include "ImGUI_Editor/SelectableProject.h"
#include "ImGUI_Editor/WindowType.h"

#include "VirtualProject/IVirtualProject.h"

#include "thread"

#include "EditorComponents/ImGUI/UserInterface.h"

#include "ImGUI_Editor/EditorWindows/StartMenuWindow.Generated.h"

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