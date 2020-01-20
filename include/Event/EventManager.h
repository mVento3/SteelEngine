#pragma once

#include "stack"
#include "vector"

#include "Event/EventObserver.h"
#include "Event/IEventManager.h"

#include "Core/Type.h"

#include "RuntimeReflection/Macro.h"

#include "Event/EventManager.Generated.h"

namespace SteelEngine {

    SE_CLASS()
    class EventManager : public IEventManager
    {
        GENERATED_BODY
    private:
        std::stack<Event::Naive*> m_Events;
        std::vector<EventObserver*> m_Observers;

        void ProcessEvents() override;
        void AddEvent(Event::Naive* event) override;

    public:
        EventManager();
        ~EventManager();

        void AddEventObserver(EventObserver* observer) override;
    };

}