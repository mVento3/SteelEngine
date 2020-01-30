#pragma once

#include "Core/IDeltaTime.h"

#include "RuntimeReflection/Macro.h"

namespace SteelEngine {

    SE_CLASS(
        SteelEngine::Reflection::ReflectionAttribute::NO_SERIALIZE
    )
    class DeltaTime : public IDeltaTime
    {
    private:
        float m_DeltaTime;
        Type::uint32 m_UPS;
        Type::uint32 m_UPS_Counter;
        float m_OneSecondTime;

    public:
        DeltaTime();
        ~DeltaTime();

        void Update() override;

        inline float GetDeltaTime() const override
        {
            return m_DeltaTime;
        }

        inline Type::uint32 GetUPS() const override
        {
            return m_UPS;
        }
    };

}