#pragma once

#include "RuntimeReflection/Macro.h"

#include "Event/NaiveEvent.h"

#include "Input/Events/KeyDownEvent.Generated.h"

namespace SteelEngine {

    SE_STRUCT(
        SteelEngine::Reflection::ReflectionAttribute::NAIVE_EVENT,
        SteelEngine::Reflection::ReflectionAttribute::NO_SERIALIZE
    )
    struct KeyDownEvent : public Event::Naive
    {
        GENERATED_BODY

        KeyDownEvent(int keyCode) :
            KeyDownEvent()
        {
            m_KeyCode = keyCode;
        }

        int m_KeyCode;
    };

}