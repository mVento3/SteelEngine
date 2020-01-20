#pragma once

#include "HotReloader/IRuntimeObject.h"

#include "Event/EventObserver.h"

namespace SteelEngine {

    struct IEventManager : public HotReloader::IRuntimeObject
    {
        friend class ICore;
    protected:
        virtual void ProcessEvents() = 0;
        virtual void AddEvent(Event::Naive* event) = 0;

    public:
        virtual void AddEventObserver(EventObserver* observer) = 0;

        template <typename A>
        void DispatchEvent(A* event)
        {
            AddEvent((Event::Naive*)event);
        }
    };

}