#pragma once

#include "VirtualProject/IVirtualProject.h"

namespace SteelEngine {

    struct LoadedProjectEvent
    {
        const IVirtualProject* m_VirtualProject;
    };

}