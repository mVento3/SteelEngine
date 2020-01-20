#pragma once

#include "chrono"

namespace SteelEngine { namespace Profiler {

    class SimpleProfiler
    {
    private:
        std::chrono::time_point<std::chrono::steady_clock> m_Start;
        std::chrono::time_point<std::chrono::steady_clock> m_Stop;
        float m_Time;

    public:
        SimpleProfiler();
        ~SimpleProfiler();

        void Start();
        void Stop();

        inline float GetTime() { return m_Time; }
    };

}}