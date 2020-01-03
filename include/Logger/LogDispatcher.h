#pragma once

#include "Logger/LogData.h"

namespace SteelEngine {

    struct LogDispatcher
    {
        virtual void Dispatch(LogData log) = 0;
    };

}