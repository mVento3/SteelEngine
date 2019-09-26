#pragma once

#include "string"
#include "vector"

namespace SteelEngine {

    struct ProjectStructure
    {
        std::string m_DirectoryName;
        std::vector<ProjectStructure> m_Other;
    };

}