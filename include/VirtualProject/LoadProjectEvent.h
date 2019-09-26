#pragma once

#include "VirtualProject/IVirtualProject.h"

namespace SteelEngine {

    struct LoadProjectEvent
    {
        std::filesystem::path m_Path;
        IVirtualProject** m_VirtualProject;
    };

}