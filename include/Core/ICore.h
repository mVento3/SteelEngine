#pragma once

#include "HotReloader/IRuntimeObject.h"

#include "Event/IEventManager.h"

namespace SteelEngine {

    struct ICore : public HotReloader::IRuntimeObject
    {
    protected:
        void ProcessEvents(IEventManager* manager) { manager->ProcessEvents(); }

    public:
        virtual void Start() { }
        virtual void Stop() { }
    };

}