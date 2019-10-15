#include "FileSystem/FileSystem.h"

#include "Module/ModuleExport.h"
#include "Module/ModuleDetails.h"

#include "Windows.h"

namespace SteelEngine {

    FileSystem::FileSystem()
    {

    }

    FileSystem::~FileSystem()
    {

    }
    
    void FileSystem::MapImpl(const std::string& name, const std::filesystem::path& dir)
    {
        m_MappedDirectories[name] = dir;
    }

    const std::filesystem::path& FileSystem::GetImpl(const std::string& name)
    {
        return m_MappedDirectories[name];
    }

}