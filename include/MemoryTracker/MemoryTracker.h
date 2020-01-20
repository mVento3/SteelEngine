#pragma once

#include "MemoryTracker/IMemoryTracker.h"

#include "RuntimeReflection/Macro.h"

#include "Memory/Internal/PoolAllocator.h"

#include "MemoryTracker/MemoryTracker.Generated.h"

namespace SteelEngine {

    SE_CLASS()
    class MemoryTracker : public IMemoryTracker
    {
        GENERATED_BODY
    public:
        struct AllocatorMap
        {
            AllocatorMap** m_Children;
            size_t m_Index;
            const Memory::Allocator* m_Allocator;
        };

    private:
        AllocatorMap* m_Allocators;
        const void* m_Memory;

        size_t m_LastID;

        Memory::Allocator* m_RootAllocator;

    public:
        MemoryTracker(Memory::Allocator* mainAllocator);
        ~MemoryTracker();

        Result Init() override;
        void Track(Memory::Allocator* allocator) override;
    };

}