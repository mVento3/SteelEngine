#pragma once

#include "Core/Type.h"
#include "Core/ReflectionAttributes.h"

#include "Event/NaiveEvent.h"

#include "RuntimeReflection/Macro.h"

#include "Window/Events/WindowResizedEvent.Generated.h"

namespace SteelEngine {

    SE_STRUCT(
        SteelEngine::ReflectionAttribute::NAIVE_EVENT,
        SteelEngine::ReflectionAttribute::NO_SERIALIZE
    )
    struct WindowResizedEvent : public Event::Naive
    {
        GENERATED_BODY

        WindowResizedEvent(Type::uint32 width, Type::uint32 height) :
            WindowResizedEvent()
        {
            m_Width = width;
            m_Height = height;
        }

        Type::uint32 m_Width;
        Type::uint32 m_Height;
    };

}