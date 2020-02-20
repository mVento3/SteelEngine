#pragma once

#include "Event/IEventManager.h"

#include "Graphics/IRenderer.h"

namespace SteelEngine {

    struct ICore
    {
    protected:
        void ProcessEvents(IEventManager* manager) { manager->ProcessEvents(); }

    public:
        virtual void Start() = 0;
        virtual void Stop() = 0;
        virtual void Update() = 0;
    };

}