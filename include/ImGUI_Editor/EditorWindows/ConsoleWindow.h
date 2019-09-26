#pragma once

#include "ImGUI_Editor/Window.h"

#include "RuntimeReflection/Macro.h"

#include "ImGUI_Editor/ReflectionAttribs.h"
#include "ImGUI_Editor/SceneType.h"

#include "ImGUI_Editor/EditorWindows/ConsoleWindow.Generated.h"

namespace SteelEngine {

    SE_CLASS(
        SteelEngine::Editor::ReflectionAttributes::SCENE_TYPE = SteelEngine::Editor::SceneType::EDITOR_SCENE | SteelEngine::Editor::SceneType::START_MENU_SCENE,
        SteelEngine::Editor::ReflectionAttributes::EDITOR_WINDOW
    )
    class ConsoleWindow : public Editor::Window
    {
        GENERATED_BODY
    private:

    public:
        ConsoleWindow();
        ~ConsoleWindow();
    };

}