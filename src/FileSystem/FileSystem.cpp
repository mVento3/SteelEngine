#include "FileSystem/FileSystem.h"

#include "Module/ModuleExport.h"
#include "Module/ModuleDetails.h"

namespace SteelEngine {

    FileSystem::FileSystem()
    {

    }

    FileSystem::~FileSystem()
    {

    }
    
    void FileSystem::MapImpl(const std::string& name, const filesystem::path& dir)
    {
        m_MappedDirectories[name] = dir;
    }

    const filesystem::path& FileSystem::GetImpl(const std::string& name)
    {
        return m_MappedDirectories[name];
    }

    void FileSystem::operator()(const RecompiledEvent& event)
    {
        
    }

}