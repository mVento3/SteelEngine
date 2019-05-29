#include "RuntimeReflection/ReflectionGenerator.h"
#include "RuntimeReflection/Reflection.h"
#include "RuntimeReflection/ReflectionAttributes.h"

#include "Module/Module.h"
#include "Module/ModuleDetails.h"

#include "ModuleManager/ModuleManager.h"

#include "RuntimeCompiler/IRuntimeObject.h"

// Only used in building .exe and .dll part to create required reflection

int main(int argc, char* argv[])
{
    std::string arg1 = argv[1];
    std::string arg2 = argv[2];
    std::string arg3 = argv[3];

    SteelEngine::ModuleManager::SetGlobalBinaryLocation(arg1);
    SteelEngine::ModuleManager::LoadAll();

    printf("Generating reflection for: %s %s\n", arg2.c_str(), arg3.c_str());

    void* dll;

#ifdef SE_WINDOWS
    SteelEngine::Module::Load("RuntimeReflection.dll", &dll);
#else
    SteelEngine::Module::Load("RuntimeReflection.so", &dll);
#endif
    SteelEngine::Module::Details* info;

    SteelEngine::Module::Get("exports", dll, (void**)&info);

    SteelEngine::Interface::IReflectionGenerator* rg = (SteelEngine::Interface::IReflectionGenerator*)info->m_AllocateCallback(0, 0);

    std::vector<SteelEngine::Interface::IRuntimeObject*> modules;
    std::vector<SteelEngine::IReflectionData*> types =
        SteelEngine::Reflection::GetTypes();

    for(SteelEngine::Type::uint32 i = 0; i < types.size(); i++)
    {
        SteelEngine::IReflectionData* type = types[i];

        if(type->GetMetaData(SteelEngine::ReflectionAttribute::SE_REFLECTION_MODULE)->Convert<bool>())
        {
            modules.push_back(type->Create());
        }
    }

    try
    {
        if(rg->Load(arg2, arg3) == SteelEngine::SE_FALSE)
        {
            printf("Failed to load!\n");

            return 0;
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

            return 0;
        }
    }
    catch(const std::exception& e)
    {
        printf("Exception while parsing in ReflectionGenerator.exe: %s!\n", e.what());
    };

    try
    {
        if(rg->Generate("bin/Runtime/GeneratedReflection") == SteelEngine::SE_FALSE)
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