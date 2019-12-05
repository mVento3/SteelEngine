#include "MemoryTracker/MemoryTracker.h"

#include "Core/Core.h"

namespace SteelEngine {

    MemoryTracker::MemoryTracker(Memory::Allocator* mainAllocator) :
        m_RootAllocator(mainAllocator)
    {
        m_LastID = 0;
        m_Memory = mainAllocator->GetStart();
    }

    MemoryTracker::~MemoryTracker()
    {

    }

    Result MemoryTracker::Init()
    {
        Reflection::GetType("SteelEngine::Core")->SetMetaData(
            Core::GlobalSystems::MEMORY_TRACKER,
            &m_Object
        );

        void* someMemory = Memory::allocateSize(*m_RootAllocator, 128 * 10);//m_RootAllocator->Allocate(128 * 10);

        Memory::PoolAllocator* pool = new(someMemory) Memory::PoolAllocator(someMemory, 128 * 10, 128);

        m_Allocators = Memory::allocate<AllocatorMap>(*pool);

        m_Allocators->m_Allocator = m_RootAllocator;
        m_Allocators->m_Children = Memory::allocateArray<AllocatorMap*>(*pool, 10);
        m_Allocators->m_Index = 0;

        return SE_TRUE;
    }

    void MemoryTracker::Track(Memory::Allocator* allocator)
    {
        // allocator->
    }

}