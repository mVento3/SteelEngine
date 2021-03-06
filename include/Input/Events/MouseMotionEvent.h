#pragma once

#include "RuntimeReflection/Macro.h"

#include "Event/NaiveEvent.h"

#include "Input/Events/MouseMotionEvent.Generated.h"

namespace SteelEngine {

    SE_STRUCT(
        SteelEngine::Reflection::ReflectionAttribute::NAIVE_EVENT,
        SteelEngine::Reflection::ReflectionAttribute::NO_SERIALIZE
    )
    struct MouseMotionEvent : public Event::Naive
    {
        GENERATED_BODY

        MouseMotionEvent(int positionX, int positionY) :
            MouseMotionEvent()
        {
            m_PositionX = positionX;
            m_PositionY = positionY;
        }

        int m_PositionX;
        int m_PositionY;
    };

}