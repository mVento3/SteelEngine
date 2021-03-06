#pragma once

#include "EditorComponents/ImGUI/UserInterface.h"

#include "Editor/ImGUI/WindowType.h"
#include "Editor/ImGUI/ReflectionAttribs.h"

#include "Editor/ImGUI/EditorWindows/SystemsMonitorWindow.Generated.h"

#include "RuntimeReflection/Reflection.h"

#include "RuntimeDatabase/RuntimeDatabase.h"

namespace SteelEngine { namespace Editor { namespace ImGUI {

    // This window will showing informations about some systems in the engine
    // like global events, reflected classes, scripts objects to clear etc.
    // To do this we will need some tracker class which should be accessable
    // from most places

    SE_CLASS(
        SteelEngine::Editor::ReflectionAttributes::EDITOR_WINDOW,
        SteelEngine::Editor::ReflectionAttributes::WINDOW_TYPE = SteelEngine::Editor::WindowType::STATIC,
        SteelEngine::Reflection::ReflectionAttribute::RUNTIME_SERIALIZE,
        SteelEngine::Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS,
        SteelEngine::EditorComponents::ImGUI::UserInterface::Attributes::SEPARATE_WINDOW,
        Reflection::ReflectionAttribute::HOT_RELOAD
    )
    class SystemsMonitorWindow : public EditorComponents::ImGUI::UserInterface, public HotReloader::IRuntimeObject
    {
        GENERATED_BODY
    private:
        int m_CurrentItem;
        int m_CurrentReflectionItem;

        std::vector<const char*> reflectionRes;

        Variant* m_Info;

        RuntimeDatabase* m_Database = 0;

    public:
        SystemsMonitorWindow();
        ~SystemsMonitorWindow();

        void Init() override;
        void Draw() override;

        void OnRecompile(HotReloader::IRuntimeObject* oldObject) override;
    };

}}}