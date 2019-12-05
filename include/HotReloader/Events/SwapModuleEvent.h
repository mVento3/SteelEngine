#pragma once

#include "string"

#include "RuntimeReflection/Macro.h"

#include "Core/ReflectionAttributes.h"

namespace SteelEngine {

    SE_CLASS(
        SteelEngine::ReflectionAttribute::NO_SERIALIZE
    )
    struct SwapModuleEvent
    {
        std::string m_ModuleName;
    };

}