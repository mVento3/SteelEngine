#include "Profiler/Manager.h"

namespace SteelEngine { namespace Profiler {

    Manager::Manager()
    {

    }

    Manager::~Manager()
    {

    }

    void Manager::AddTime(TimeData time)
    {
        m_Timings.push_back(time);
    }

    std::vector<Manager::TimeData>* Manager::GetTime()
    {
        return &m_Timings;
    }

}}