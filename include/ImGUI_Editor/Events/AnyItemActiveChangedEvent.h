#pragma once

#include "RuntimeReflection/Macro.h"

#include "Event/NaiveEvent.h"

#include "ImGUI_Editor/Events/AnyItemActiveChangedEvent.Generated.h"

namespace SteelEngine {

    SE_STRUCT(
        SteelEngine::Reflection::ReflectionAttribute::NAIVE_EVENT,
        SteelEngine::Reflection::ReflectionAttribute::NO_SERIALIZE
    )
    struct AnyItemActiveChangedEvent : public Event::Naive
    {
        GENERATED_BODY

        AnyItemActiveChangedEvent(bool state) :
            AnyItemActiveChangedEvent()
        {
            m_State = state;
        }

        bool m_State;
    };

}