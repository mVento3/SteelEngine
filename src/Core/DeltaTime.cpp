#include "Core/DeltaTime.h"

#include "chrono"

namespace SteelEngine {

    DeltaTime::DeltaTime()
    {
        m_UPS = 0;
        m_DeltaTime = 0;
        m_OneSecondTime = 0;
        m_UPS_Counter = 0;
    }

    DeltaTime::~DeltaTime()
    {

    }

    void DeltaTime::Update()
    {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        m_DeltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        startTime = currentTime;

        if(m_OneSecondTime >= 1.f)
        {
            m_UPS = m_UPS_Counter;
            m_UPS_Counter = 0;
            m_OneSecondTime = 0;
        }

        m_OneSecondTime += m_DeltaTime;
        m_UPS_Counter++;
    }

}