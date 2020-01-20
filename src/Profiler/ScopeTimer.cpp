#include "Profiler/ScopeTimer.h"

#include "RuntimeReflection/Reflection.h"

#include "Core/Core.h"

#include "Profiler/Manager.h"

namespace SteelEngine { namespace Profiler {

    ScopeTimer::ScopeTimer(const std::string& name, const std::filesystem::path& file, size_t line) :
        m_Name(name),
        m_File(file),
        m_Line(line)
    {
        m_Start = std::chrono::high_resolution_clock::now();
    }

    ScopeTimer::~ScopeTimer()
    {
        m_Stop = std::chrono::high_resolution_clock::now();

        static Manager** manager = Reflection::GetType("SteelEngine::Core")->GetMetaData(Core::GlobalSystems::PROFILER)->Convert<Manager**>();
        float time = std::chrono::duration<float, std::chrono::milliseconds::period>(m_Stop - m_Start).count();

        Reflection::GetType((*manager))->Invoke("AddTime", (*manager), Manager::TimeData{ time, m_Name, m_File, m_Line });
    }

}}