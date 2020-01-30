#pragma once

#include "RuntimeReflection/Macro.h"

#include "Event/NaiveEvent.h"

#include "Window/Events/WindowMinimizedEvent.Generated.h"

namespace SteelEngine {

    SE_STRUCT(
        SteelEngine::Reflection::ReflectionAttribute::NAIVE_EVENT,
        SteelEngine::Reflection::ReflectionAttribute::NO_SERIALIZE
    )
    struct WindowMinimizedEvent : public Event::Naive
    {
        GENERATED_BODY
    };

}