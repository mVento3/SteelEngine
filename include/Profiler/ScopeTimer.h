#pragma once

#include "chrono"
#include "string"

namespace SteelEngine { namespace Profiler {

    class ScopeTimer
    {
    private:
        std::chrono::time_point<std::chrono::steady_clock> m_Start;
        std::chrono::time_point<std::chrono::steady_clock> m_Stop;
        std::string m_Name;

    public:
        ScopeTimer(const std::string& name);
        ~ScopeTimer();
    };

}}

#ifdef SE_PROFILE
#define SE_PROFILE_SCOPE(name) SteelEngine::Profiler::ScopeTimer scope ## __LINE__ ## (name)
#else
#define SE_PROFILE_SCOPE(name)
#endif

#define SE_PROFILE_FUNC SE_PROFILE_SCOPE(__FUNCSIG__)