#include "RuntimeReflection/ReflectionGenerator.h"
#include "RuntimeReflection/Reflection.h"

#include "Core/ReflectionAttributes.h"

#include "Module/Module.h"
#include "Module/ModuleDetails.h"

#include "ModuleManager/ModuleManager.h"

#include "HotReloader/IRuntimeObject.h"

#include "Utils/Options.h"

#include "FileSystem/FileSystem.h"

#include "InformationTracker/InformationTracker.h"

#include "Memory/Container/Vector.h"

enum Index
{
    INCLUDE_FILE,
    BIN_LOCATION,
    CURRENT_WORKING_DIR
};

std::vector<SteelEngine::Options::Descriptor> desc =
{
    { INCLUDE_FILE, "i" },
    { BIN_LOCATION, "b" },
    { CURRENT_WORKING_DIR, "cwd" }
};

int main(int argc, char* argv[])
{
    SteelEngine::Options parser(argv, argc, desc);

    SteelEngine::Reflection::Init();

    SteelEngine::Reflection::GetType("SteelEngine::Core")->SetMetaData(
        SteelEngine::ReflectionAttribute::SYSTEMS_INFORMATION_TRACKER,
        new SteelEngine::InformationTracker()
    );

    printf("Generating reflection for: %s\n", parser[INCLUDE_FILE].m_Result.c_str());

    void* dll;

// #ifdef SE_WINDOWS
//     SteelEngine::Module::Load("RuntimeReflection.dll", &dll);
// #else
//     SteelEngine::Module::Load("RuntimeReflection.so", &dll);
// #endif
//     SteelEngine::Module::Details* info;

//     SteelEngine::Module::Get("exports", dll, (void**)&info);

//     SteelEngine::IReflectionGenerator* rg = (SteelEngine::IReflectionGenerator*)info->m_AllocateCallback(0, 0);

    SteelEngine::IReflectionGenerator* rg = new SteelEngine::ReflectionGenerator();

    std::vector<SteelEngine::HotReloader::IRuntimeObject*> modules;
    SteelEngine::IReflectionData** types = SteelEngine::Reflection::GetTypes();

    for(SteelEngine::Type::uint32 i = 0; i < SteelEngine::Reflection::GetTypesSize(); i++)
    {
        SteelEngine::IReflectionData* type = types[i];

        if(type->GetMetaData(SteelEngine::ReflectionAttribute::REFLECTION_MODULE)->Convert<bool>())
        {
            modules.push_back(type->Create());
        }
    }

    try
    {
        if(rg->Load(parser[CURRENT_WORKING_DIR].m_Result + "/" + parser[INCLUDE_FILE].m_Result) == SteelEngine::SE_FALSE)
        {
            printf("Failed to load!\n");

            return 0;
        }
    }
    catch(const std::exception& e)
    {
        printf("Exception while loading in ReflectionGenerator.exe: %s!\n", e.what());
    }

    // try
    // {
        if(rg->Parse() == SteelEngine::SE_FALSE)
        {
            printf("Failed to parse!\n");

            return 0;
        }
    // }
    // catch(const std::exception& e)
    // {
    //     printf("Exception while parsing in ReflectionGenerator.exe: %s!\n", e.what());
    // };

    try
    {
        if(rg->Generate(parser[CURRENT_WORKING_DIR].m_Result, parser[CURRENT_WORKING_DIR].m_Result + "/build/GeneratedReflection") == SteelEngine::SE_FALSE)
        {
            printf("Failed to generate!\n");

            return 0;
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

    delete rg;
    rg = 0;

    return 0;
}