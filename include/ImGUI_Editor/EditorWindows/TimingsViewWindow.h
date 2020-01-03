#pragma once

#include "RuntimeReflection/Macro.h"
#include "Core/ReflectionAttributes.h"

#include "ImGUI_Editor/ReflectionAttribs.h"
#include "ImGUI_Editor/SceneType.h"

#include "ImGUI_Editor/EditorWindows/ColumnHeader.h"

#include "EditorComponents/ImGUI/UserInterface.h"

#include "Profiler/Manager.h"

#include "ImGUI_Editor/EditorWindows/TimingsViewWindow.Generated.h"

namespace SteelEngine {

    SE_CLASS(
        SteelEngine::Editor::ReflectionAttributes::SCENE_TYPE = SteelEngine::Editor::SceneType::EDITOR_SCENE | SteelEngine::Editor::SceneType::START_MENU_SCENE,
        SteelEngine::Editor::ReflectionAttributes::EDITOR_WINDOW,
        SteelEngine::ReflectionAttribute::GENERATE_CAST_FUNCTIONS,
        SteelEngine::EditorComponents::ImGUI::UserInterface::Attributes::SEPARATE_WINDOW,
        SteelEngine::ReflectionAttribute::RUNTIME_SERIALIZE
    )
    class TimingsViewWindow : public EditorComponents::ImGUI::UserInterface
    {
        GENERATED_BODY
    private:
        Profiler::IManager** m_Manager;
        std::vector<ColumnHeader> m_Headers;
        IReflectionData* m_Type;

    public:
        TimingsViewWindow();
        ~TimingsViewWindow();

        void Init() override;
        void Draw() override;
    };

}