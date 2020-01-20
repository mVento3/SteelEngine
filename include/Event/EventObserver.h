#pragma once

#include "Event/NaiveEvent.h"

namespace SteelEngine {

    struct EventObserver
    {
        virtual void OnEvent(Event::Naive* event) = 0;
    };

}