#pragma once

#include "filesystem"

namespace SteelEngine {

    struct GetLoadedProjectPathEvent
    {
        std::filesystem::path m_Path;
    };

}