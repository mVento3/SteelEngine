#pragma once

#include "EditorComponents/ImGUI/UserInterface.h"

#include "RuntimeReflection/Macro.h"

#include "ImGUI_Editor/ReflectionAttribs.h"
#include "ImGUI_Editor/SceneType.h"

#include "Logger/LogDispatcher.h"

#include "ImGUI_Editor/EditorWindows/ConsoleWindow.Generated.h"

namespace SteelEngine {

    SE_CLASS(
        SteelEngine::Editor::ReflectionAttributes::SCENE_TYPE = SteelEngine::Editor::SceneType::EDITOR_SCENE | SteelEngine::Editor::SceneType::START_MENU_SCENE,
        SteelEngine::Editor::ReflectionAttributes::EDITOR_WINDOW,
        SteelEngine::ReflectionAttribute::GENERATE_CAST_FUNCTIONS,
        SteelEngine::EditorComponents::ImGUI::UserInterface::Attributes::SEPARATE_WINDOW,
        SteelEngine::ReflectionAttribute::RUNTIME_SERIALIZE
    )
    class ConsoleWindow : public EditorComponents::ImGUI::UserInterface, public LogDispatcher
    {
        GENERATED_BODY
    private:
        std::vector<LogData> m_Logs;

    public:
        ConsoleWindow();
        ~ConsoleWindow();

        void Init() override;
        void Draw() override;

        void Dispatch(LogData log) override;

        void OnRecompile(HotReloader::IRuntimeObject* oldObject) override;
    };

}