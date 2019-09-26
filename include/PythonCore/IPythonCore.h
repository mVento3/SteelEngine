#pragma once

#include "RuntimeCompiler/IRuntimeObject.h"

#include "Core/Result.h"

namespace SteelEngine { namespace Script {

    struct IPython : public HotReload::IRuntimeObject
    {
        virtual Result Init() { return SE_NOT_IMPLEMENTED; }
        virtual void Finalize() = 0;
    };

}}