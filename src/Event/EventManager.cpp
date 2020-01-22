#include "Event/EventManager.h"

namespace SteelEngine {

    void EventManager::ProcessEvents()
    {
        while(!m_Events.empty())
        {
            Event::Naive* event = m_Events.front();

            for(Type::uint32 i = 0; i < m_Observers.size(); i++)
            {
                EventObserver* observer = m_Observers[i];

                observer->OnEvent(event);
            }

            delete event;

            m_Events.pop();
        }
    }

    void EventManager::AddEvent(Event::Naive* event)
    {
        m_Events.push(event);
    }

    EventManager::EventManager()
    {

    }

    EventManager::~EventManager()
    {

    }

    void EventManager::AddEventObserver(EventObserver* observer)
    {
        m_Observers.push_back(observer);
    }

}