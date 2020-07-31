#pragma once

#include "EditorComponents/ImGUI/UserInterface.h"

#include "HotReloader/IRuntimeObject.h"

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/Reflection.h"

#include "Editor/ImGUI/ImGUI_Editor.h"
#include "Editor/ImGUI/WindowType.h"

#include "imgui/imgui.h"

#include "Editor/ImGUI/EditorWindows/FileBrowserWindow.Generated.h"

namespace SteelEngine {

    SE_CLASS(
        Editor::ReflectionAttributes::WINDOW_TYPE = SteelEngine::Editor::WindowType::STATIC,
        Editor::ReflectionAttributes::EDITOR_WINDOW,
        Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS,
        EditorComponents::ImGUI::UserInterface::Attributes::SEPARATE_WINDOW,
        Reflection::ReflectionAttribute::RUNTIME_SERIALIZE,
        Reflection::ReflectionAttribute::HOT_RELOAD
    )
    class FileBrowserWindow :
        public EditorComponents::ImGUI::UserInterface,
        public HotReloader::IRuntimeObject
    {
        GENERATED_BODY
    private:
        std::filesystem::path m_WorkingPath;

    public:
        FileBrowserWindow();
        ~FileBrowserWindow();

        // static const char* ms_Separator;
        // static ImVec2 ms_SeparatorSize;

        void Init() override;
        void Draw() override;
    };

}