#pragma once

#include "RuntimeCompiler/IRuntimeObject.h"

#include "RuntimeReflection/Macro.h"

#include "Core/ReflectionAttributes.h"

#include "ImGUI_Editor/Window.Generated.h"

#include "imgui/imgui.h"

namespace SteelEngine { namespace Editor {

    SE_STRUCT(SteelEngine::ReflectionAttribute::RUNTIME_SERIALIZE)
    struct Window : public HotReload::IRuntimeObject
    {
        GENERATED_BODY

        std::string m_Title;
        void* m_Context;

        virtual void Init() { }
        virtual void Draw() { }
    };

}}