#pragma once

#include "Utils/Json.h"

#include "RuntimeReflection/Macro.h"

#include "Core/ReflectionAttributes.h"

namespace SteelEngine {

    SE_STRUCT(
        SteelEngine::ReflectionAttribute::NO_SERIALIZE
    )
    struct GetCompileConfigEvent
    {
        Utils::json m_Config;
    };

}