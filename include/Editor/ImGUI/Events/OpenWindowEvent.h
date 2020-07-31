#pragma once

#include "HotReloader/InheritanceTrackKeeper.h"

namespace SteelEngine {

    struct OpenWindowEvent
    {
        HotReloader::InheritanceTrackKeeper* m_Window;
        std::string m_Name;
    };

}