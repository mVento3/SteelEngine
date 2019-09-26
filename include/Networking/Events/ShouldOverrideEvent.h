#pragma once

#include "string"

#include "RuntimeReflection/Macro.h"

#include "Core/ReflectionAttributes.h"

namespace SteelEngine { namespace Network {

    SE_CLASS(
        SteelEngine::ReflectionAttribute::NO_SERIALIZE
    )
    struct ShouldOverrideEvent
    {
        std::string m_File;

        bool m_IsSet = false;
        bool m_ShouldOverride = false;
        bool m_ShouldOverrideForAll = false;
    };

}}