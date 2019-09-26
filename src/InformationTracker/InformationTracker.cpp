#include "InformationTracker/InformationTracker.h"

namespace SteelEngine {

    InformationTracker::InformationTracker()
    {

    }

    InformationTracker::~InformationTracker()
    {

    }

    void InformationTracker::Add(ISystemTracker* tracker)
    {
        m_Systems.push_back(tracker);
    }

    ISystemTracker* InformationTracker::Get(size_t typeID)
    {
        for(ISystemTracker* tracker : m_Systems)
        {
            if(tracker->GetTypeID() == typeID)
            {
                return tracker;
            }
        }

        return 0;
    }

}