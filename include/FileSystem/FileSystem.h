#pragma once

#include "FileSystem/IFileSystem.h"

#include "Core/Platform.h"

#include "RuntimeReflection/Reflection.h"

#include "ModuleManager/ModuleManager.h"

#include "RuntimeReflection/Macro.h"

#include "FileSystem.Generated.h"

#include "map"

namespace SteelEngine {

    SE_CLASS()
    class FileSystem : public Interface::IFileSystem
    {
        GENERATED_BODY
    private:
        filesystem::path m_BinaryLocation;
        std::map<std::string, filesystem::path> m_MappedDirectories;

    public:
        FileSystem();
        ~FileSystem();

        void MapImpl(const std::string& name, const filesystem::path& dir) override;
        const filesystem::path& GetImpl(const std::string& name) override;

        static void SetBinaryLocation(const std::string& binaryLocation)
        {
            static RuntimeDatabase* rd = (RuntimeDatabase*)ModuleManager::GetModule("RuntimeDatabase");
            static FileSystem* fs = (FileSystem*)rd->m_FileSystem;

            if(binaryLocation != "")
            {
                fs->m_BinaryLocation = filesystem::current_path().string() + "/" + binaryLocation;
            }
            else
            {
                fs->m_BinaryLocation = filesystem::current_path();
            }
        }

        static const filesystem::path& GetBinaryLocation()
        {
            static RuntimeDatabase* rd = (RuntimeDatabase*)ModuleManager::GetModule("RuntimeDatabase");
            static FileSystem* fs = (FileSystem*)rd->m_FileSystem;

            return fs->m_BinaryLocation;
        }

        static void Map(const std::string& name, const filesystem::path& dir)
        {
            static RuntimeDatabase* rd = (RuntimeDatabase*)ModuleManager::GetModule("RuntimeDatabase");
            static FileSystem* fs = (FileSystem*)rd->m_FileSystem;

            fs->MapImpl(name, dir);
        }

        static const filesystem::path& Get(const std::string& name, const filesystem::path& dir)
        {
            static RuntimeDatabase* rd = (RuntimeDatabase*)ModuleManager::GetModule("RuntimeDatabase");
            static FileSystem* fs = (FileSystem*)rd->m_FileSystem;

            return fs->GetImpl(name);
        }
    };

}