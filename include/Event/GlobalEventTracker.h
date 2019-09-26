#pragma once

#include "Event/GlobalEvent.h"

#include "InformationTracker/SystemTracker.h"

#include "vector"

namespace SteelEngine {

    class GlobalEventTracker : public SystemTracker<Event::GlobalEvent>
    {
    public:
        struct Info
        {
            Info(const std::string& name, const std::string& value) :
                m_ObjectName(name),
                m_EventType(value)
            {

            }

            std::string m_ObjectName;
            std::string m_EventType;
        };

    private:
        std::vector<Info> m_Info;

    public:
        GlobalEventTracker();
        ~GlobalEventTracker();

        void SomeFunc(const std::string& name, const std::string& value);
        std::vector<Info>& GetAll();
    };

}