#pragma once

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/IReflectionMethod.h"

#include "Editor/ImGUI/ReflectionAttribs.h"
#include "Editor/ImGUI/WindowType.h"

#include "Editor/ImGUI/EditorWindows/ColumnHeader.h"

#include "EditorComponents/ImGUI/UserInterface.h"

#include "Profiler/Manager.h"

#include "Core/IDeltaTime.h"

#include "Editor/ImGUI/EditorWindows/TimingsViewWindow.Generated.h"

namespace SteelEngine {

    SE_CLASS(
        SteelEngine::Editor::ReflectionAttributes::WINDOW_TYPE = SteelEngine::Editor::WindowType::STATIC,
        SteelEngine::Editor::ReflectionAttributes::EDITOR_WINDOW,
        SteelEngine::Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS,
        SteelEngine::EditorComponents::ImGUI::UserInterface::Attributes::SEPARATE_WINDOW,
        SteelEngine::Reflection::ReflectionAttribute::RUNTIME_SERIALIZE,
        Reflection::ReflectionAttribute::HOT_RELOAD
    )
    class TimingsViewWindow : public EditorComponents::ImGUI::UserInterface, public HotReloader::IRuntimeObject
    {
        GENERATED_BODY
    private:
        Profiler::IManager** m_Manager;
        IReflectionData* m_Type;
        const IReflectionMethod* m_GetTimeMethod;
        std::vector<Profiler::Manager::TimeData>* m_Timings;

    public:
        TimingsViewWindow();
        ~TimingsViewWindow();

        void Init() override;
        void Draw() override;
    };

}