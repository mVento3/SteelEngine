#pragma once

#include "RuntimeReflection/Macro.h"

#include "Core/ReflectionAttributes.h"

#include "Event/NaiveEvent.h"

#include "Window/Events/WindowMinimizedEvent.Generated.h"

namespace SteelEngine {

    SE_STRUCT(
        SteelEngine::ReflectionAttribute::NAIVE_EVENT,
        SteelEngine::ReflectionAttribute::NO_SERIALIZE
    )
    struct WindowMinimizedEvent : public Event::Naive
    {
        GENERATED_BODY
    };

}