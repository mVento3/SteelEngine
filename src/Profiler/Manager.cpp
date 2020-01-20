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
        for(Type::uint32 i = 0; i < m_Timings.size(); i++)
        {
            TimeData& data = m_Timings[i];

            if(data.m_Name == time.m_Name)
            {
                data.m_Time = time.m_Time;

                return;
            }
        }

        m_Timings.push_back(time);
    }

    std::vector<Manager::TimeData>* Manager::GetTime()
    {
        return &m_Timings;
    }

}}