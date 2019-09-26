#include "Event/GlobalEventTracker.h"

namespace SteelEngine {

    GlobalEventTracker::GlobalEventTracker()
    {

    }

    GlobalEventTracker::~GlobalEventTracker()
    {

    }

    void GlobalEventTracker::SomeFunc(const std::string& name, const std::string& value)
    {
        m_Info.push_back(Info(name, value));
    }

    std::vector<GlobalEventTracker::Info>& GlobalEventTracker::GetAll()
    {
        return m_Info;
    }

}