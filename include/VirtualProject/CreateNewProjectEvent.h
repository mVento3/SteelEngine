#pragma once

#include "VirtualProject/IVirtualProject.h"
#include "VirtualProject/ProjectStructure.h"

namespace SteelEngine {

    struct CreateNewProjectEvent
    {
        CreateNewProjectEvent(const ProjectStructure& project, const std::filesystem::path& path) :
            m_Structure(project),
            m_Path(path)
        {

        }

        IVirtualProject** m_Project;
        ProjectStructure m_Structure;
        std::filesystem::path m_Path;
    };

}