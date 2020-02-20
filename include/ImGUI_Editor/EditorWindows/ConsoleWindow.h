#pragma once

#include "EditorComponents/ImGUI/UserInterface.h"

#include "RuntimeReflection/Macro.h"

#include "ImGUI_Editor/ReflectionAttribs.h"
#include "ImGUI_Editor/SceneType.h"

#include "Logger/LogDispatcher.h"

#include "imgui/imgui.h"

#include "ConsoleSystem/IConsoleSystem.h"

#include "Event/EventObserver.h"

#include "ImGUI_Editor/EditorWindows/ConsoleWindow.Generated.h"

namespace SteelEngine {

    SE_CLASS(
        Editor::ReflectionAttributes::SCENE_TYPE = SteelEngine::Editor::SceneType::EDITOR_SCENE | SteelEngine::Editor::SceneType::START_MENU_SCENE,
        Editor::ReflectionAttributes::EDITOR_WINDOW,
        Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS,
        EditorComponents::ImGUI::UserInterface::Attributes::SEPARATE_WINDOW,
        EditorComponents::ImGUI::UserInterface::Attributes::FLAGS = ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar,
        Reflection::ReflectionAttribute::RUNTIME_SERIALIZE,
        Reflection::ReflectionAttribute::HOT_RELOAD
    )
    class ConsoleWindow :
        public EditorComponents::ImGUI::UserInterface,
        public LogDispatcher,
        public EventObserver,
        public HotReloader::IRuntimeObject
    {
        GENERATED_BODY
    private:
        std::vector<LogData> m_Logs;

        bool m_ScrollDown;
        char* m_CommandBuffer;
        const Type::uint32 m_CommandBufferSize = 1024;

        bool m_Keys[256] = { false };

        IConsoleSystem** m_ConsoleSystem;

    public:
        ConsoleWindow();
        ~ConsoleWindow();

        void Init() override;
        void Draw() override;

        void Dispatch(LogData log) override;
        void OnEvent(Event::Naive* event) override;

        void OnRecompile(HotReloader::IRuntimeObject* oldObject) override;
    };

}