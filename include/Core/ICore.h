#pragma once

#include "RuntimeCompiler/IRuntimeObject.h"

namespace SteelEngine {

    struct ICore : public HotReload::IRuntimeObject
    {
        virtual void Start() { }
        virtual void Stop() { }
    };

}