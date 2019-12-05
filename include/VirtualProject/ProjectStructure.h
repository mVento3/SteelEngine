#pragma once

#include "string"
#include "vector"

namespace SteelEngine {

    struct ProjectStructure
    {
        ProjectStructure(const std::string& name, const std::string& content = "", bool forceFile = false) :
            m_DirectoryName(name),
            m_Content(content)
        {
            if(content != "" && !forceFile)
            {
                m_ForceFile = true;
            }
        }

        ProjectStructure(const std::string& name, bool forceFile) :
            m_DirectoryName(name),
            m_ForceFile(forceFile)
        {

        }

        ProjectStructure(const std::string& name, const std::vector<ProjectStructure>& other) :
            m_DirectoryName(name),
            m_Other(other)
        {

        }

        std::string m_DirectoryName;
        std::string m_Content = "";
        std::vector<ProjectStructure> m_Other;
        bool m_ForceFile = false;
    };

}