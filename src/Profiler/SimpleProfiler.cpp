#include "Profiler/SimpleProfiler.h"

namespace SteelEngine { namespace Profiler {

    SimpleProfiler::SimpleProfiler()
    {
        m_Time = 0;
    }

    SimpleProfiler::~SimpleProfiler()
    {

    }

    void SimpleProfiler::Start()
    {
        m_Start = std::chrono::high_resolution_clock::now();
    }

    void SimpleProfiler::Stop()
    {
        m_Stop = std::chrono::high_resolution_clock::now();
        m_Time = std::chrono::duration<float, std::chrono::milliseconds::period>(m_Stop - m_Start).count();
    }

}}