#pragma once

#include "RuntimeCompiler/IRuntimeObject.h"

#include "Utils/Window.Generated.h"

#include "RuntimeReflection/Macro.h"

#include "Core/ReflectionAttributes.h"

namespace SteelEngine { namespace Utils {

    SE_STRUCT(
        SteelEngine::ReflectionAttribute::RUNTIME_SERIALIZE
    )
    struct Window : public HotReload::IRuntimeObject
    {
        GENERATED_BODY

        std::string m_Title;
        void* m_Context;

        virtual void Init() { }
        virtual void Draw() { }
    };

}}