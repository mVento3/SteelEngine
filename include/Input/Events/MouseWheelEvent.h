#pragma once

#include "RuntimeReflection/Macro.h"

#include "Event/NaiveEvent.h"

#include "Input/Events/MouseWheelEvent.Generated.h"

namespace SteelEngine {

    SE_STRUCT(
        SteelEngine::Reflection::ReflectionAttribute::NAIVE_EVENT,
        SteelEngine::Reflection::ReflectionAttribute::NO_SERIALIZE
    )
    struct MouseWheelEvent : public Event::Naive
    {
        GENERATED_BODY

        MouseWheelEvent(float y) :
            MouseWheelEvent()
        {
            m_Y = y;
        }

        float m_Y;
    };

}