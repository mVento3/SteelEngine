#pragma once

#include "RuntimeCompiler/IRuntimeObject.h"

#include "RuntimeReflection/Macro.h"

#include "Core/ReflectionAttributes.h"

#include "ImGUI_Editor/Window.Generated.h"

#include "imgui/imgui.h"

namespace SteelEngine { namespace Editor {

    namespace ImGUI {

        class ImGUI_Editor;

    }

    SE_STRUCT(SteelEngine::ReflectionAttribute::RUNTIME_SERIALIZE)
    struct Window : public HotReload::IRuntimeObject
    {
        GENERATED_BODY

        std::string m_Title;
        void* m_Context;
        ImGUI::ImGUI_Editor** m_Editor;

        virtual void Init() { }
        virtual void Draw() { }

        virtual void OnRecompile(HotReload::IRuntimeObject* oldObject) override;
    };

}}