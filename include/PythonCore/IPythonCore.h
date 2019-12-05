#pragma once

#include "HotReloader/IRuntimeObject.h"

#include "Core/Result.h"

namespace SteelEngine { namespace Script {

    struct IPython : public HotReloader::IRuntimeObject
    {
        virtual Result Init() { return SE_NOT_IMPLEMENTED; }
        virtual void Finalize() = 0;
    };

}}