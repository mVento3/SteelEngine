#pragma once

#include "RuntimeReflection/Macro.h"

#include "Event/NaiveEvent.h"

#include "Core/ReflectionAttributes.h"

#include "Window/Events/WindowMaximizedEvent.Generated.h"

namespace SteelEngine {

    SE_STRUCT(
        SteelEngine::ReflectionAttribute::NAIVE_EVENT,
        SteelEngine::ReflectionAttribute::NO_SERIALIZE
    )
    struct WindowMaximizedEvent : public Event::Naive
    {
        GENERATED_BODY
    };

}