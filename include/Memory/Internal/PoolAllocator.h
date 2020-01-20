#pragma once

#include "Memory/Allocator.h"
#include "Memory/PointerMath.h"
#include "Memory/StackLinkedList.h"

namespace SteelEngine { namespace Memory { namespace Internal {

    class PoolAllocator : public Allocator
    {
    public:
        struct Header
        {

        };

        using Node = StackLinkedList<Header>::Node;

    protected:
        size_t m_ChunkSize;
        StackLinkedList<Header> m_FreeList;

    public:
        PoolAllocator(void* startAddress, size_t count, size_t chunkSize);
        PoolAllocator(size_t count, size_t chunkSize);
        PoolAllocator(size_t count, size_t chunkSize, Allocator* allocator);
        ~PoolAllocator();

        void* Allocate(size_t size, Type::uint8 alignment) override;
        void Deallocate(void* ptr) override;

        void Reset();
    };

}}}