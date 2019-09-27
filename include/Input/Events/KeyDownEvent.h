#pragma once

#include "RuntimeReflection/Macro.h"

#include "Core/ReflectionAttributes.h"
#include "Core/Type.h"

namespace SteelEngine {

    SE_STRUCT(
        SteelEngine::ReflectionAttribute::NO_SERIALIZE
    )
    struct KeyDownEvent
    {
        int m_KeyCode;
    };

}