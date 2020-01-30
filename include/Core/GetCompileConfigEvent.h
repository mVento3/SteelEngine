#pragma once

#include "Utils/Json.h"

#include "RuntimeReflection/Macro.h"

namespace SteelEngine {

    SE_STRUCT(
        SteelEngine::Reflection::ReflectionAttribute::NO_SERIALIZE
    )
    struct GetCompileConfigEvent
    {
        Utils::json m_Config;
    };

}