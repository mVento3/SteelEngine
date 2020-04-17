#pragma once

#include "EditorComponents/ImGUI/UserInterface.h"

#include "HotReloader/IRuntimeObject.h"

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/Reflection.h"

#include "ImGUI_Editor/ImGUI_Editor.h"
#include "ImGUI_Editor/WindowType.h"

#include "imgui/imgui.h"

#include "ImGUI_Editor/EditorWindows/FileBrowserWindow.Generated.h"

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