#pragma once

#include "RuntimeReflection/Macro.h"

#include "HotReloader/IRuntimeObject.h"

#include "EditorComponents/ImGUI/UserInterface.Generated.h"

namespace SteelEngine {

    namespace Editor { namespace ImGUI {

    class ImGUI_Editor;

    }}

    namespace EditorComponents { namespace ImGUI {

    SE_CLASS(
        SteelEngine::Reflection::ReflectionAttribute::RUNTIME_SERIALIZE
    )
    class UserInterface : public HotReloader::IRuntimeObject
    {
        GENERATED_BODY
    public:
        enum Attributes
        {
            // Draw separate window for class
            SEPARATE_WINDOW,
            FLAGS
        };

    protected:

    public:
        UserInterface();
        ~UserInterface();

        void* m_Context;
        char m_Title[64];
        Editor::ImGUI::ImGUI_Editor** m_Editor;

        virtual void Init();
        virtual void Draw();

        virtual void OnRecompile(HotReloader::IRuntimeObject* oldObject) override;
    };

}}}