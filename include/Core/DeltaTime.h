#pragma once

#include "Core/IDeltaTime.h"

#include "RuntimeReflection/Macro.h"

#include "Core/ReflectionAttributes.h"

namespace SteelEngine {

    SE_CLASS(
        SteelEngine::ReflectionAttribute::NO_SERIALIZE
    )
    class DeltaTime : public IDeltaTime
    {
    private:
        float m_DeltaTime;

    public:
        DeltaTime();
        ~DeltaTime();

        void Update() override;

        inline float GetDeltaTime() const override { return m_DeltaTime; }
    };

}