#include "Memory/Internal/PoolAllocator.h"

#include "assert.h"

namespace SteelEngine { namespace Memory { namespace Internal {

    PoolAllocator::PoolAllocator(void* startAddress, size_t count, size_t chunkSize) :
        Allocator(count * (chunkSize + sizeof(Node)), PointerMath::add(startAddress, sizeof(PoolAllocator))),
        m_ChunkSize(chunkSize + sizeof(Node))
    {
        Reset();
    }

    PoolAllocator::PoolAllocator(size_t count, size_t chunkSize) :
        Allocator(count * (chunkSize + sizeof(Node)), PointerMath::add(this, sizeof(PoolAllocator))),
        m_ChunkSize(chunkSize + sizeof(Node))
    {
        Reset();
    }

    PoolAllocator::PoolAllocator(size_t count, size_t chunkSize, Allocator* allocator) :
        Allocator(count * (chunkSize + sizeof(Node)), PointerMath::add(allocator->Allocate(count * (chunkSize + sizeof(Node)) + sizeof(PoolAllocator), __alignof(PoolAllocator)), sizeof(PoolAllocator))),
        m_ChunkSize(chunkSize + sizeof(Node))
    {
        Reset();
    }

    PoolAllocator::~PoolAllocator()
    {

    }

    void* PoolAllocator::Allocate(size_t size, Type::uint8 alignment)
    {
        assert(size <= m_ChunkSize && "The size is too big!");

        Node* freeAddress = m_FreeList.Pop();

        m_UsedMemorySize += m_ChunkSize;
        m_AllocationsCount++;

        return freeAddress;
    }

    void PoolAllocator::Deallocate(void* ptr)
    {
        m_FreeList.Push((Node*)ptr);

        m_UsedMemorySize -= m_ChunkSize;
        m_AllocationsCount--;
    }

    void PoolAllocator::Reset()
    {
        m_UsedMemorySize = 0;
        m_AllocationsCount = 0;

        const size_t nChunks = m_MemorySize / m_ChunkSize;

        for(size_t i = 0; i < nChunks; i++)
        {
            Type::uptr address = (Type::uptr)m_Start + i * m_ChunkSize;
            m_FreeList.Push((Node*)address);
        }
    }

}}}