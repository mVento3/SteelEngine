#include "Module/Module.h"
#include "Module/ModuleDetails.h"

#include "FileWatcher/FileWatcher.h"

#include "Core/Core.h"

#include "RuntimeReflection/Reflection.h"

#include "ModuleManager/ModuleManager.h"

#include "FileSystem/FileSystem.h"

#include "Utils/Options.h"

#include "InformationTracker/InformationTracker.h"

#include "Memory/LinearAllocator.h"

#include "stack"
#include "fstream"

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

// struct TestComponent : SteelEngine::EntityComponentSystem::ECS::Component
// {
//     int x;
//     int y;

//     TestComponent(int x, int y) :
//         x(x),
//         y(y)
//     {

//     }
// };

// struct TestComponent2 : SteelEngine::EntityComponentSystem::ECS::Component
// {
//     int posX;
//     int posY;

//     TestComponent2(int x, int y) :
//         posX(x),
//         posY(y)
//     {

//     }
// };

// void a(SteelEngine::EntityComponentSystem::ECS::Component* components[32], size_t count)
// {
//     using Component = SteelEngine::EntityComponentSystem::ECS::Component;

//     for(size_t i = 0; i < count; i++)
//     {
//         Component* component = components[i];

//         if(component->m_ComponentTypeID == SteelEngine::EntityComponentSystem::ECS::GetComponentTypeID<TestComponent2>())
//         {
//             TestComponent2* com = (TestComponent2*)component;

//             com->posX += 1;
//             com->posY += 1;

//             // printf("AA\n");
//         }

//         if(component->m_ComponentTypeID == SteelEngine::EntityComponentSystem::ECS::GetComponentTypeID<TestComponent>())
//         {
//             TestComponent* com = (TestComponent*)component;

//             com->x -= 1;
//             com->y -= 1;

//             // printf("AA\n");
//         }
//     }
// }

int main(int argc, char* argv[])
{
// Get additional application arguments/options
    SteelEngine::Options parser(argv, argc, desc);

// Allocate main/root memory chunk
    size_t rootMemorySize = 1024 * 1024LL * 512;
    void* rootMemory = malloc(rootMemorySize);
	SteelEngine::Memory::Allocator* rootMemoryAllocator;

// Setup root memory allocator
    void* paddedAddress = SteelEngine::Memory::PointerMath::calculateForwardAddress((SteelEngine::Type::uptr)rootMemory, __alignof(SteelEngine::Memory::LinearAllocator));
    rootMemoryAllocator = new(paddedAddress) SteelEngine::Memory::LinearAllocator(rootMemorySize - sizeof(SteelEngine::Memory::LinearAllocator));

    // SteelEngine::EntityComponentSystem::ECS ecs(rootMemoryAllocator);

    // auto entity = ecs.Create();

    // ecs.Assign<TestComponent2>(entity, 22, 33);
    // ecs.Assign<TestComponent>(entity, 10, 20);

    // ecs.Each<TestComponent, TestComponent2>(&a);

// Load the  database and set root memory arguments
    SteelEngine::ModuleManager::Load(getBinaryLocation() / "RuntimeDatabase.dll");

    SteelEngine::RuntimeDatabase* db = (SteelEngine::RuntimeDatabase*)SteelEngine::ModuleManager::GetModule("RuntimeDatabase");

    db->m_RootMemory = rootMemory;
    db->m_RootMemorySize = rootMemorySize;
    db->m_RootMemoryAllocator = rootMemoryAllocator;

// Initialize rest of allocators inside database
    db->Init();

// Load and initialize reflection
    SteelEngine::Reflection::Init();

    SteelEngine::IReflectionData* type = SteelEngine::Reflection::GetType("SteelEngine::Core");

    if(!type->GetMetaData(SteelEngine::Core::GlobalSystems::GLOBAL_EVENTS)->IsValid())
    {
        type->SetMetaData(SteelEngine::Core::GlobalSystems::GLOBAL_EVENTS, std::unordered_map<size_t, void*>());
    }

    SteelEngine::Reflection::GetType("SteelEngine::Core")->SetMetaData(
        SteelEngine::ReflectionAttribute::SYSTEMS_INFORMATION_TRACKER,
        new SteelEngine::InformationTracker()
    );

    SteelEngine::HotReloader::IRuntimeObject* core = SteelEngine::Reflection::CreateInstance("SteelEngine::Core");

    SteelEngine::Reflection::GetType("SteelEngine::Core")->SetMetaData(
        SteelEngine::Core::GlobalSystems::FILE_SYSTEM,
        (SteelEngine::IFileSystem*)SteelEngine::Reflection::CreateInstance("SteelEngine::FileSystem")
    );

    // Then after that we need to prepare file system
    SteelEngine::FileSystem::Map("bin", getBinaryLocation());
    SteelEngine::FileSystem::Map("se_init_log", getBinaryLocation() / "se_init_log.log");

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