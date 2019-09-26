#pragma once

#include "string"

#include "RuntimeReflection/Macro.h"

namespace SteelEngine {

    SE_CLASS()
    struct SwapModuleEvent
    {
        std::string m_ModuleName;
    };

}