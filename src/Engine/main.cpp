#include "Module/Module.h"
#include "Module/ModuleDetails.h"

#include "FileWatcher/FileWatcher.h"

#include "Core/Core.h"
#include "Core/Platform.h"

#include "RuntimeReflection/Reflection.h"

#include "ModuleManager/ModuleManager.h"

#include "FileSystem/FileSystem.h"

#include "fstream"

#include "Utils/Options.h"

enum Index
{
    SERVER,
    BIN_LOCATION,
    IP,
    ENGINE_DEV,
    GAME_DEV
};

std::vector<SteelEngine::Options::Descriptor> desc =
{
    { SERVER, "s" },
    { BIN_LOCATION, "p" },
    { IP, "ip" },
    { ENGINE_DEV, "ed" },
    { GAME_DEV, "gd" }
};

int main(int argc, char* argv[])
{
    SteelEngine::Options parser(argv, argc, desc);

    // We need to prepare modules for engine
    SteelEngine::ModuleManager::SetGlobalBinaryLocation(parser[BIN_LOCATION].m_Result);
    SteelEngine::ModuleManager::LoadAll();

    SteelEngine::RuntimeDatabase* rd = (SteelEngine::RuntimeDatabase*)SteelEngine::ModuleManager::GetModule("RuntimeDatabase");

    rd->m_FileSystem = (SteelEngine::Interface::IFileSystem*)SteelEngine::Reflection::CreateInstance("FileSystem");

    // Then after that we need to prepare file system
    SteelEngine::FileSystem::SetBinaryLocation(parser[BIN_LOCATION].m_Result);
    SteelEngine::FileSystem::Map("bin", SteelEngine::FileSystem::GetBinaryLocation());

    SteelEngine::Interface::IRuntimeObject* core = SteelEngine::Reflection::CreateInstance("Core");

    if(parser[SERVER].m_IsSet)
    {
        SteelEngine::Reflection::GetType("Core")->SetMetaData(SteelEngine::Core::IS_SERVER, true);
    }
    else
    {
        SteelEngine::Reflection::GetType("Core")->SetMetaData(SteelEngine::Core::IS_SERVER, false);
        SteelEngine::Reflection::GetType("Core")->SetMetaData(SteelEngine::Core::SERVER_IP, parser[IP].m_Result);
    }

    if(!parser[ENGINE_DEV].m_IsSet && !parser[GAME_DEV].m_IsSet)
    {
        printf("Please specify path variant like -gd for game dev of -ed for engine dev!\n");

        return 1;
    }
    else if(parser[ENGINE_DEV].m_IsSet)
    {
        SteelEngine::Reflection::GetType("Core")->Invoke("SetPathVariant", core, SteelEngine::Core::EnginePathVariant::ENGINE_DEV);
    }
    else if(parser[GAME_DEV].m_IsSet)
    {
        SteelEngine::Reflection::GetType("Core")->Invoke("SetPathVariant", core, SteelEngine::Core::EnginePathVariant::GAME_DEV);
    }

    SteelEngine::Reflection::GetType("Core")->Invoke("Start", core);

    while(1) { }

    return EXIT_SUCCESS;
}