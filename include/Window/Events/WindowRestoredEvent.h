#pragma once

#include "RuntimeReflection/Macro.h"

#include "Event/NaiveEvent.h"

#include "Window/Events/WindowRestoredEvent.Generated.h"

namespace SteelEngine {

    SE_STRUCT(
        Reflection::ReflectionAttribute::NAIVE_EVENT,
        Reflection::ReflectionAttribute::NO_SERIALIZE
    )
    struct WindowRestoredEvent : public Event::Naive
    {
        GENERATED_BODY
    };

}