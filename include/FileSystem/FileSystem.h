#pragma once

#include "FileSystem/IFileSystem.h"

#include "RuntimeReflection/Reflection.h"

#include "ModuleManager/ModuleManager.h"

#include "RuntimeReflection/Macro.h"

#include "FileSystem/FileSystem.Generated.h"

#include "map"

namespace SteelEngine {

    SE_CLASS()
    class FileSystem : public IFileSystem
    {
        GENERATED_BODY
    private:
        std::map<std::string, std::filesystem::path> m_MappedDirectories;

    public:
        FileSystem();
        ~FileSystem();

        void MapImpl(const std::string& name, const std::filesystem::path& dir) override;
        const std::filesystem::path& GetImpl(const std::string& name) override;

        static void Map(const std::string& name, const std::filesystem::path& dir)
        {
            static RuntimeDatabase* rd = (RuntimeDatabase*)ModuleManager::GetModule("RuntimeDatabase");
            static FileSystem* fs = (FileSystem*)rd->m_FileSystem;

            fs->MapImpl(name, dir);
        }

        static const std::filesystem::path& Get(const std::string& name)
        {
            static RuntimeDatabase* rd = (RuntimeDatabase*)ModuleManager::GetModule("RuntimeDatabase");
            static FileSystem* fs = (FileSystem*)rd->m_FileSystem;

            return fs->GetImpl(name);
        }
    };

}