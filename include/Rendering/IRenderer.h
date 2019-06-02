#pragma once

#include "RuntimeCompiler/IRuntimeObject.h"

#include "Core/Result.h"

namespace SteelEngine { namespace Interface {

    struct IRenderer : public IRuntimeObject
    {
        virtual Result Init() { return SE_NOT_IMPLEMENTED; }
    };

}}