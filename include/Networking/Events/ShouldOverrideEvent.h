#pragma once

#include "string"

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine { namespace Network {

    SE_CLASS(
        SteelEngine::Reflection::ReflectionAttribute::NO_SERIALIZE
    )
    struct ShouldOverrideEvent
    {
        std::string m_File;

        bool m_IsSet = false;
        bool m_ShouldOverride = false;
        bool m_ShouldOverrideForAll = false;
    };

}}