#pragma once

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/Reflection.h"

#include "HotReloader/IRuntimeObject.h"

#include "EditorComponents/ImGUI/UserInterface.Generated.h"

namespace SteelEngine {

    namespace Editor { namespace ImGUI {

    class ImGUI_Editor;

    }}

    namespace EditorComponents { namespace ImGUI {

    SE_CLASS(
        Reflection::ReflectionAttribute::RUNTIME_SERIALIZE,
        Reflection::ReflectionAttribute::GENERATE_OWN_SERIALIZE_FUNC
    )
    class UserInterface
    {
        GENERATED_BODY
    public:
        enum Attributes
        {
            // Draw separate window for class
            SEPARATE_WINDOW,
            FLAGS
        };

    private:
        bool m_Draw;

    protected:

    public:
        UserInterface();
        ~UserInterface();

        void* m_Context;
        char m_Title[64];
        Editor::ImGUI::ImGUI_Editor** m_Editor;
        size_t m_TypeID;

        virtual void Init();
        virtual void Draw() { }
        void DrawUser();

        virtual void OnRecompile(HotReloader::IRuntimeObject* oldObject);
        // void OnSwap(HotReloader::SwapStage stage, HotReloader::ObjectAge age);
    };

}}}