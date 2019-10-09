#pragma once

#include "FileSystem/IFileSystem.h"

#include "RuntimeReflection/Reflection.h"

#include "ModuleManager/ModuleManager.h"

#include "RuntimeReflection/Macro.h"

#include "FileSystem/FileSystem.Generated.h"

#include "map"

#include "Core/Core.h"

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
            static IFileSystem* fs = Reflection::GetType("SteelEngine::Core")->GetMetaData(SteelEngine::Core::GlobalSystems::FILE_SYSTEM)->Convert<IFileSystem*>();

            fs->MapImpl(name, dir);
        }

        static const std::filesystem::path& Get(const std::string& name)
        {
            static IFileSystem* fs = Reflection::GetType("SteelEngine::Core")->GetMetaData(SteelEngine::Core::GlobalSystems::FILE_SYSTEM)->Convert<IFileSystem*>();

            return fs->GetImpl(name);
        }
    };

}