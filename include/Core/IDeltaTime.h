#pragma once

#include "HotReloader/IRuntimeObject.h"

namespace SteelEngine {

    struct IDeltaTime : public HotReloader::IRuntimeObject
    {
        friend class Core;
    protected:
        virtual void Update() = 0;

    public:
        virtual float GetDeltaTime() const = 0;
        virtual Type::uint32 GetUPS() const = 0;
    };

}