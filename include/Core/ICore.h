#pragma once

#include "HotReloader/IRuntimeObject.h"

#include "Event/IEventManager.h"

#include "Graphics/IRenderer.h"

namespace SteelEngine {

    struct ICore : public HotReloader::IRuntimeObject
    {
    protected:
        void ProcessEvents(IEventManager* manager) { manager->ProcessEvents(); }

    public:
        virtual void Start() = 0;
        virtual void Stop() = 0;
    };

}