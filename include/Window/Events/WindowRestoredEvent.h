#pragma once

#include "RuntimeReflection/Macro.h"

#include "Event/NaiveEvent.h"

#include "Core/ReflectionAttributes.h"

#include "Window/Events/WindowRestoredEvent.Generated.h"

namespace SteelEngine {

    SE_STRUCT(
        SteelEngine::ReflectionAttribute::NAIVE_EVENT,
        SteelEngine::ReflectionAttribute::NO_SERIALIZE
    )
    struct WindowRestoredEvent : public Event::Naive
    {
        GENERATED_BODY
    };

}