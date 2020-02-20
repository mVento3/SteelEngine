#pragma once

#include "Core/Type.h"

namespace SteelEngine {

    struct IDeltaTime
    {
        friend class Core;
    protected:
        virtual void Update() = 0;

    public:
        virtual float GetDeltaTime() const = 0;
        virtual Type::uint32 GetUPS() const = 0;
    };

}