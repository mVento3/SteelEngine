#include "RuntimeDatabase/RuntimeDatabase.h"

#include "Module/ModuleExport.h"

#include "RuntimeReflection/ReflectionData.h"

SE_PLUGIN(SteelEngine::RuntimeDatabase, "RuntimeDatabase", SteelEngine::Module::Details::PluginType::CORE_PLUGIN, SteelEngine::Module::Details::PluginFlag::ONCE)

namespace SteelEngine {

    RuntimeDatabase::RuntimeDatabase()
    {
        m_TypesSize = 0;

        m_RootMemorySize = 1024 * 1024LL;

        m_RootMemory = malloc(m_RootMemorySize);
        void* paddedAddress = Memory::PointerMath::calculateForwardAddress((Type::uptr)m_RootMemory, __alignof(Memory::LinearAllocator));
        m_RootMemoryAllocator = new(paddedAddress) Memory::LinearAllocator(m_RootMemorySize - sizeof(Memory::LinearAllocator));

        m_AvailablePerVariantIDs = Memory::allocate<Container::Stack<size_t>>(*m_RootMemoryAllocator, SE_MAX_VARIANTS, m_RootMemoryAllocator);
        m_AvailablePerObjectIDs = Memory::allocate<Container::Stack<size_t>>(*m_RootMemoryAllocator, SE_MAX_VARIANTS, m_RootMemoryAllocator);

        for(size_t i = 0; i < SE_MAX_VARIANTS; i++)
        {
            m_AvailablePerVariantIDs->Push(i);
        }

        for(size_t i = 0; i < SE_MAX_OBJECTS; i++)
        {
            m_AvailablePerObjectIDs->Push(i);
        }

        m_TypesAllocator = Memory::allocate<Memory::PoolAllocator>(*m_RootMemoryAllocator, SE_MAX_TYPES * SE_MAX_TYPE_SIZE, SE_MAX_TYPE_SIZE, m_RootMemoryAllocator);
        m_Types = Memory::allocateArray<IReflectionData*>(*m_RootMemoryAllocator, SE_MAX_TYPES);

        m_Objects = Memory::allocate<Container::Vector<ConstrucedObject>>(*m_RootMemoryAllocator, SE_MAX_OBJECTS, m_RootMemoryAllocator);

        m_VariantsAllocator = Memory::allocate<Memory::PoolAllocator>(*m_RootMemoryAllocator, SE_MAX_VARIANTS * SE_MAX_VARIANT_SIZE, SE_MAX_VARIANT_SIZE, m_RootMemoryAllocator);
    }

    RuntimeDatabase::~RuntimeDatabase()
    {

    }

    size_t RuntimeDatabase::GetNextPerVariantID()
    {
        if(m_AvailablePerVariantIDs->Empty())
        {
            return s_InvalidID;
        }

        size_t id = m_AvailablePerVariantIDs->Top();

        m_AvailablePerVariantIDs->Pop();

        return id;
    }

    void RuntimeDatabase::PushPerVariantID(size_t id)
    {
        if(id != s_InvalidID)
        {
            m_AvailablePerVariantIDs->Push(id);
        }
    }

    size_t RuntimeDatabase::GetNextPerObjectID()
    {
        if(m_AvailablePerObjectIDs->Empty())
        {
            return s_InvalidID;
        }

        size_t id = m_AvailablePerObjectIDs->Top();

        m_AvailablePerObjectIDs->Pop();

        return id;
    }

    void RuntimeDatabase::PushPerObjectID(size_t id)
    {
        if(id != s_InvalidID)
        {
            m_AvailablePerObjectIDs->Push(id);
        }
    }

}