#pragma once

#include "RuntimeReflection/Macro.h"

#include "Event/NaiveEvent.h"

#include "Core/ReflectionAttributes.h"

#include "Input/Events/KeyUpEvent.Generated.h"

namespace SteelEngine {

    SE_STRUCT(
        SteelEngine::ReflectionAttribute::NAIVE_EVENT,
        SteelEngine::ReflectionAttribute::NO_SERIALIZE
    )
    struct KeyUpEvent : public Event::Naive
    {
        GENERATED_BODY

        KeyUpEvent(int keyCode) :
            KeyUpEvent()
        {
            m_KeyCode = keyCode;
        }

        int m_KeyCode;
    };

}