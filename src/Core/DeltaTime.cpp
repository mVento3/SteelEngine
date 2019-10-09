#include "Core/DeltaTime.h"

#include "chrono"

namespace SteelEngine {

    DeltaTime::DeltaTime()
    {

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
    }

}