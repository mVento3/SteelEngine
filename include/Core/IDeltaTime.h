#pragma once

#include "RuntimeCompiler/IRuntimeObject.h"

namespace SteelEngine {

    struct IDeltaTime : public HotReload::IRuntimeObject
    {
        friend class Core;
    protected:
        virtual void Update() = 0;

    public:
        virtual float GetDeltaTime() const = 0;
    };

}