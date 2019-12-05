#pragma once

#include "Core/ReflectionAttributes.h"

#include "RuntimeReflection/Macro.h"

#include "ImGUI_Editor/ReflectionAttribs.h"
#include "ImGUI_Editor/Window.h"
#include "ImGUI_Editor/ImGUI_Editor.h"
#include "ImGUI_Editor/SelectableProject.h"
#include "ImGUI_Editor/ChangeSceneEvent.h"

#include "VirtualProject/IVirtualProject.h"

#include "thread"

#include "EditorComponents/ImGUI/UserInterface.h"

#include "ImGUI_Editor/EditorWindows/StartMenuWindow.Generated.h"

namespace SteelEngine { namespace Editor { namespace ImGUI {

    SE_CLASS(
        SteelEngine::Editor::ReflectionAttributes::EDITOR_WINDOW,
        SteelEngine::Editor::ReflectionAttributes::SCENE_TYPE = SteelEngine::Editor::SceneType::START_MENU_SCENE,
        SteelEngine::ReflectionAttribute::RUNTIME_SERIALIZE,
        SteelEngine::ReflectionAttribute::GENERATE_CAST_FUNCTIONS,
        SteelEngine::EditorComponents::ImGUI::UserInterface::Attributes::SEPARATE_WINDOW
    )
    class StartMenuWindow : public SteelEngine::EditorComponents::ImGUI::UserInterface
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