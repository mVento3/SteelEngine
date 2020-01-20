#include "RuntimeDatabase/RuntimeDatabase.h"

#include "Module/ModuleExport.h"

#include "RuntimeReflection/ReflectionData.h"
#include "RuntimeReflection/MetaDataInfo.h"

SE_PLUGIN(SteelEngine::RuntimeDatabase, "RuntimeDatabase", SteelEngine::Module::Details::PluginType::CORE_PLUGIN, SteelEngine::Module::Details::PluginFlag::ONCE)

namespace SteelEngine {

    void RuntimeDatabase::Reflection::Init(Memory::Allocator* allocator)
    {
        m_TypesSize = 0;
        m_TypesAllocator = Memory::allocate<Memory::Internal::PoolAllocator>(*allocator, SE_MAX_TYPES, SE_MAX_TYPE_SIZE, allocator);
        m_Types = Memory::allocateArray<IReflectionData*>(*allocator, SE_MAX_TYPES);

        // entt::registry entt;

        // for(Type::uint32 i = 0; i < EntityComponentSystem::EntityManager::MAX_ENTITIES; i++)
        // {
        //     auto entity = entt.create();

        //     entt.assign<Test>(entity);
        //     entt.assign<Test2>(entity);
        //     entt.assign<Test3>(entity);
        // }

        // std::chrono::time_point<std::chrono::steady_clock> start2 = std::chrono::high_resolution_clock::now();

        // entt.view<Test, Test2, Test3>().each([](auto& a, auto& b, auto& c)
        // {
        //     a.x += 0.1f;
        // });

        // auto view = entt.view<Test, Test2, Test3>();

        // for(auto ent : view)
        // {
        //     auto& a = view.get<Test>(ent);
        //     auto& b = view.get<Test2>(ent);
        //     auto& c = view.get<Test3>(ent);

        //     a.x += 0.1f;
        // }

        // std::chrono::time_point<std::chrono::steady_clock> stop2 = std::chrono::high_resolution_clock::now();
        // float time2 = std::chrono::duration<float, std::chrono::milliseconds::period>(stop2 - start2).count();

        // EntityComponentSystem::ECS ecs;

        // ecs.RegisterComponent<Test>();
        // ecs.RegisterComponent<Test2>();
        // ecs.RegisterComponent<Test3>();

        // auto p = ecs.RegisterSystem<TestS>();

        // EntityComponentSystem::EntityManager::Signature s;

        // s.set(ecs.GetComponentType<Test>());
        // s.set(ecs.GetComponentType<Test2>());
        // s.set(ecs.GetComponentType<Test3>());

        // ecs.SetSystemSignature<TestS>(s);

        // EntityComponentSystem::EntityManager::EntityType entities[EntityComponentSystem::EntityManager::MAX_ENTITIES];

        // for(Type::uint32 i = 0; i < EntityComponentSystem::EntityManager::MAX_ENTITIES; i++)
        // {
        //     entities[i] = ecs.CreateEntity();

        //     ecs.AddComponent(entities[i], Test{});
        //     ecs.AddComponent(entities[i], Test2{});
        //     ecs.AddComponent(entities[i], Test3{});
        // }

        // std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::high_resolution_clock::now();

        // p->Update(ecs);

        // std::chrono::time_point<std::chrono::steady_clock> stop = std::chrono::high_resolution_clock::now();
        // float time = std::chrono::duration<float, std::chrono::milliseconds::period>(stop - start).count();

        // printf("\n");

        // m_MetaData = Memory::allocateArray<MetaDataInfo**>(*allocator, MetaDataGroup::SIZE);
        // m_MetaDataCount = 0;

        // for(Type::uint32 i = 0; i < MetaDataGroup::SIZE; i++)
        // {
        //     m_MetaData[i] = Memory::allocateArray<MetaDataInfo*>(*allocator, SE_MAX_TYPES);

        //     MetaDataInfo** arr = m_MetaData[i];

        //     for(Type::uint32 k = 0; k < SE_MAX_TYPES; k++)
        //     {
        //         arr[k] = Memory::allocateArray<MetaDataInfo>(*allocator, 16);
        //     }
        // }
    }

    void RuntimeDatabase::HotReloader::Init(Memory::Allocator* allocator)
    {
        m_Objects = Memory::allocate<Container::Vector<ConstrucedObject>>(*allocator, SE_MAX_OBJECTS, allocator);
        m_AvailablePerObjectIDs = Memory::allocate<Container::Stack<size_t>>(*allocator, SE_MAX_VARIANTS, allocator);

        for(size_t i = 0; i < SE_MAX_OBJECTS; i++)
        {
            m_AvailablePerObjectIDs->Push(i);
        }
    }

    void RuntimeDatabase::Variant::Init(Memory::Allocator* allocator)
    {
        m_AvailablePerVariantIDs = Memory::allocate<Container::Stack<size_t>>(*allocator, SE_MAX_VARIANTS, allocator);
        m_VariantsAllocator = Memory::allocate<Memory::Internal::PoolAllocator>(*allocator, SE_MAX_VARIANTS, SE_MAX_VARIANT_SIZE, allocator);

        for(size_t i = 0; i < SE_MAX_VARIANTS; i++)
        {
            m_AvailablePerVariantIDs->Push(i);
        }
    }

    void RuntimeDatabase::Init()
    {    
        m_ReflectionDatabase = Memory::allocate<RuntimeDatabase::Reflection>(*m_RootMemoryAllocator);
        m_HotReloaderDatabase = Memory::allocate<RuntimeDatabase::HotReloader>(*m_RootMemoryAllocator);
        m_VariantDatabase = Memory::allocate<RuntimeDatabase::Variant>(*m_RootMemoryAllocator);

        m_VariantDatabase->Init(m_RootMemoryAllocator);
        m_ReflectionDatabase->Init(m_RootMemoryAllocator);
        m_HotReloaderDatabase->Init(m_RootMemoryAllocator);
    }

    RuntimeDatabase::RuntimeDatabase()
    {

    }

    RuntimeDatabase::~RuntimeDatabase()
    {

    }

    size_t RuntimeDatabase::GetNextPerVariantID()
    {
        if(m_VariantDatabase->m_AvailablePerVariantIDs->Empty())
        {
            return s_InvalidID;
        }

        size_t id = m_VariantDatabase->m_AvailablePerVariantIDs->Top();

        m_VariantDatabase->m_AvailablePerVariantIDs->Pop();

        return id;
    }

    void RuntimeDatabase::PushPerVariantID(size_t id)
    {
        if(id != s_InvalidID)
        {
            m_VariantDatabase->m_AvailablePerVariantIDs->Push(id);
        }
    }

    size_t RuntimeDatabase::GetNextPerObjectID()
    {
        if(m_HotReloaderDatabase->m_AvailablePerObjectIDs->Empty())
        {
            return s_InvalidID;
        }

        size_t id = m_HotReloaderDatabase->m_AvailablePerObjectIDs->Top();

        m_HotReloaderDatabase->m_AvailablePerObjectIDs->Pop();

        return id;
    }

    void RuntimeDatabase::PushPerObjectID(size_t id)
    {
        if(id != s_InvalidID)
        {
            m_HotReloaderDatabase->m_AvailablePerObjectIDs->Push(id);
        }
    }

}