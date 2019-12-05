#pragma once

#include "HotReloader/IRuntimeObject.h"

namespace SteelEngine {

    struct ICore : public HotReloader::IRuntimeObject
    {
        virtual void Start() { }
        virtual void Stop() { }
    };

}