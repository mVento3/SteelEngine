#pragma once

#include "Memory/Allocator.h"
#include "Memory/PointerMath.h"
#include "Memory/StackLinkedList.h"

namespace SteelEngine { namespace Memory {

    class PoolAllocator : public Allocator
    {
    public:
        struct Header
        {

        };

    private:
        using Node = StackLinkedList<Header>::Node;

        size_t m_ChunkSize;
        StackLinkedList<Header> m_FreeList;

    public:
        PoolAllocator(void* startAddress, size_t maxSize, size_t chunkSize);
        ~PoolAllocator();

        void* Allocate(size_t size, Type::uint8 alignment) override;
        void Deallocate(void* ptr) override;

        void Reset();
    };

}}