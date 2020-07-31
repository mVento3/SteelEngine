#include "RuntimeReflection/Reflection.h"

#include "Module/Module.h"
#include "Module/ModuleDetails.h"

#include "ModuleManager/ModuleManager.h"

#include "HotReloader/IRuntimeObject.h"

#include "Utils/Options.h"

#include "FileSystem/FileSystem.h"

#include "InformationTracker/InformationTracker.h"

#include "Memory/Container/Vector.h"

#include "RuntimeReflection/ReflectionGenerator.h"

enum Index
{
    CURRENT_WORKING_DIR,
    BIN_DIR,
    INCLUDE_DIR
};

std::vector<SteelEngine::Options::Descriptor> desc =
{
    { CURRENT_WORKING_DIR, "cwd" },
    { BIN_DIR, "bin" },
    { INCLUDE_DIR, "i" }
};

void generateReflectionForFile(SteelEngine::IReflectionGenerator* rg, const std::filesystem::path& cwd, const std::filesystem::path& header)
{
    try
    {
        if(rg->Load(header) == SteelEngine::SE_FALSE)
        {
            printf("Failed to load!\n");

            return;
        }
    }
    catch(const std::exception& e)
    {
        printf("Exception while loading in ReflectionGenerator.exe: %s!\n", e.what());
    }

    try
    {
        if(rg->Parse() == SteelEngine::SE_FALSE)
        {
            printf("Failed to parse!\n");

            return;
        }
    }
    catch(const std::exception& e)
    {
        printf("Exception while parsing in ReflectionGenerator.exe: %s!\n", e.what());
    };

    try
    {
        if(rg->Generate(cwd, cwd / "build/GeneratedReflection") == SteelEngine::SE_FALSE)
        {
            printf("Failed to generate!\n");

            return;
        }
    }
    catch(const std::exception& e)
    {
        printf("Exception while generating in ReflectionGenerator.exe: %s!\n", e.what());
    };

    try
    {
        rg->Clear();
    }
    catch(const std::exception& e)
    {
        printf("Exception while clearing in ReflectionGenerator.exe: %s!\n", e.what());
    };
}

int main(int argc, char* argv[])
{
    SteelEngine::Options parser(argv, argc, desc);

// Allocate main/root memory chunk
    size_t rootMemorySize = 4096 * 1024LL;
    void* rootMemory = malloc(rootMemorySize);
	SteelEngine::Memory::Allocator* rootMemoryAllocator;

// Setup root memory allocator
    void* paddedAddress = SteelEngine::Memory::PointerMath::calculateForwardAddress((SteelEngine::Type::uptr)rootMemory, __alignof(SteelEngine::Memory::LinearAllocator));
    rootMemoryAllocator = new(paddedAddress) SteelEngine::Memory::LinearAllocator(rootMemorySize - sizeof(SteelEngine::Memory::LinearAllocator));

// Load the runtime database and set root memory arguments
    SteelEngine::ModuleManager::Load(getBinaryLocation() / "RuntimeDatabase.dll");

    SteelEngine::RuntimeDatabase* db = (SteelEngine::RuntimeDatabase*)SteelEngine::ModuleManager::GetModule("RuntimeDatabase");

    db->m_RootMemory = rootMemory;
    db->m_RootMemorySize = rootMemorySize;
    db->m_RootMemoryAllocator = rootMemoryAllocator;

// Initialize rest of allocators inside database
    db->Init();

    SteelEngine::Reflection::Init();

    SteelEngine::Reflection::GetType("SteelEngine::Core")->SetMetaData(
        SteelEngine::Reflection::ReflectionAttribute::SYSTEMS_INFORMATION_TRACKER,
        new SteelEngine::InformationTracker()
    );

    SteelEngine::IReflectionGenerator* rg = (SteelEngine::IReflectionGenerator*)SteelEngine::Reflection::CreateInstance("SteelEngine::ReflectionGenerator");

    if(!parser[INCLUDE_DIR].m_IsSet)
    {
        std::filesystem::path cwd = parser[CURRENT_WORKING_DIR].m_Result;

        for(auto path : std::filesystem::recursive_directory_iterator(cwd / "include"))
        {
            if(!path.is_directory() && path.path().extension() == ".h")
            {
                printf("Generating reflection for: %s\n", path.path().string().c_str());

                generateReflectionForFile(rg, cwd, path);
            }
        }
    }
    else
    {
        generateReflectionForFile(rg, parser[CURRENT_WORKING_DIR].m_Result, parser[CURRENT_WORKING_DIR].m_Result + "/" + parser[INCLUDE_DIR].m_Result);
    }

    delete rg;
    rg = 0;

    return 0;
}