#pragma once

#include "RuntimeReflection/Macro.h"

#include "Core/ReflectionAttributes.h"

namespace SteelEngine {

    SE_STRUCT(
        SteelEngine::ReflectionAttribute::NO_SERIALIZE
    )
    struct MouseMotionEvent
    {
        int m_X;
        int m_Y;
    };

}