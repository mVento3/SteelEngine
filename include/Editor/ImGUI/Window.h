#pragma once

#include "HotReloader/IRuntimeObject.h"

#include "RuntimeReflection/Macro.h"

#include "Editor/ImGUI/Window.Generated.h"

#include "imgui/imgui.h"

namespace SteelEngine { namespace Editor {

    namespace ImGUI {

        class ImGUI_Editor;

    }

    SE_STRUCT(
        SteelEngine::Reflection::ReflectionAttribute::RUNTIME_SERIALIZE,
        Reflection::ReflectionAttribute::HOT_RELOAD
    )
    struct Window : public HotReloader::IRuntimeObject
    {
        GENERATED_BODY

        std::string m_Title;
        void* m_Context;
        ImGUI::ImGUI_Editor** m_Editor;

        virtual void Init() { }
        virtual void Draw() { }

        virtual void OnRecompile(HotReloader::IRuntimeObject* oldObject) override;
    };

}}