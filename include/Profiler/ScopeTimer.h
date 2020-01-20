#pragma once

#include "chrono"
#include "string"
#include "filesystem"

namespace SteelEngine { namespace Profiler {

    class ScopeTimer
    {
    private:
        std::chrono::time_point<std::chrono::steady_clock> m_Start;
        std::chrono::time_point<std::chrono::steady_clock> m_Stop;
        std::string m_Name;

        std::filesystem::path m_File;
        size_t m_Line;

    public:
        ScopeTimer(const std::string& name, const std::filesystem::path& file, size_t line);
        ~ScopeTimer();
    };

}}

#ifdef SE_PROFILE
#define SE_PROFILE_SCOPE(name) SteelEngine::Profiler::ScopeTimer scope ## __LINE__ ## (name, __FILE__, __LINE__)
#else
#define SE_PROFILE_SCOPE(name)
#endif

#define SE_PROFILE_FUNC SE_PROFILE_SCOPE(__FUNCSIG__)