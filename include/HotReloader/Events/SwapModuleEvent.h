#pragma once

#include "string"

#include "RuntimeReflection/Macro.h"

namespace SteelEngine {

    SE_CLASS(
        SteelEngine::Reflection::ReflectionAttribute::NO_SERIALIZE
    )
    struct SwapModuleEvent
    {
        std::string m_ModuleName;
    };

}