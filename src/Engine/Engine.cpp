#include "Module/Module.h"
#include "Module/ModuleDetails.h"

#include "FileWatcher/FileWatcher.h"

#include "Core/Core.h"

#include "RuntimeReflection/Reflection.h"

#include "ModuleManager/ModuleManager.h"

#include "FileSystem/FileSystem.h"

#include "fstream"

#include "Utils/Options.h"

#include "InformationTracker/InformationTracker.h"

enum Index
{
    SERVER,
    IP,
    ENGINE_DEV,
    GAME_DEV
};

std::vector<SteelEngine::Options::Descriptor> desc =
{
    { SERVER, "s" },
    { IP, "ip" },
    { ENGINE_DEV, "ed" },
    { GAME_DEV, "gd" }
};

int main(int argc, char* argv[])
{
    SteelEngine::Options parser(argv, argc, desc);

    // We need to prepare modules for engine
    SteelEngine::ModuleManager::LoadAll();

    SteelEngine::HotReload::IRuntimeObject* ooo = SteelEngine::Reflection::CreateInstance("SteelEngine::InformationTracker");

    SteelEngine::Reflection::GetType("SteelEngine::Core")->SetMetaData(
        SteelEngine::ReflectionAttribute::SYSTEMS_INFORMATION_TRACKER,
        new SteelEngine::InformationTracker()
    );

    SteelEngine::HotReload::IRuntimeObject* core = SteelEngine::Reflection::CreateInstance("SteelEngine::Core");

    SteelEngine::Reflection::GetType("SteelEngine::Core")->SetMetaData(
        SteelEngine::Core::GlobalSystems::FILE_SYSTEM,
        (SteelEngine::Interface::IFileSystem*)SteelEngine::Reflection::CreateInstance("SteelEngine::FileSystem")
    );

    // Then after that we need to prepare file system
    SteelEngine::FileSystem::Map("bin", getBinaryLocation());

    if(!parser[ENGINE_DEV].m_IsSet && !parser[GAME_DEV].m_IsSet)
    {
        printf("Please specify path variant like -gd for game dev of -ed for engine dev!\n");

        return 1;
    }
    else if(parser[ENGINE_DEV].m_IsSet)
    {
        SteelEngine::Reflection::GetType("SteelEngine::Core")->Invoke("SetPathVariant", core, SteelEngine::Core::EnginePathVariant::ENGINE_DEV);
        SteelEngine::FileSystem::Map("fileWatcherPath", getBinaryLocation().parent_path());
    }
    else if(parser[GAME_DEV].m_IsSet)
    {
        SteelEngine::Reflection::GetType("SteelEngine::Core")->Invoke("SetPathVariant", core, SteelEngine::Core::EnginePathVariant::GAME_DEV);
    }

    SteelEngine::Reflection::GetType("SteelEngine::Core")->Invoke("Start", core);

    while(1) { }

    return EXIT_SUCCESS;
}