#pragma once

#include "RuntimeReflection/Macro.h"

namespace SteelEngine {

    SE_STRUCT(
        SteelEngine::Reflection::ReflectionAttribute::NO_SERIALIZE
    )
    struct ChangeMousePositionEvent
    {
        int m_X;
        int m_Y;
    };

}