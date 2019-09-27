#pragma once

#include "RuntimeReflection/Macro.h"

#include "Core/ReflectionAttributes.h"
#include "Core/Type.h"

namespace SteelEngine {

    SE_STRUCT(
        SteelEngine::ReflectionAttribute::NO_SERIALIZE
    )
    struct KeyUpEvent
    {
        int m_KeyCode;
    };
        
}