#include "Memory/ProxyAllocator.h"

namespace SteelEngine { namespace Memory {

    ProxyAllocator::ProxyAllocator(Allocator& allocator) :
        Allocator(allocator.GetMemorySize(), allocator.GetStart()),
        m_Allocator(allocator)
    {

    }

    ProxyAllocator::~ProxyAllocator()
    {

    }

    void* ProxyAllocator::Allocate(size_t size, Type::uint8 alignment)
    {
        size_t usedMemory = m_Allocator.GetUsedMemorySize();
        void* p = m_Allocator.Allocate(size, alignment);

        m_UsedMemorySize += m_Allocator.GetUsedMemorySize() - usedMemory;
        m_AllocationsCount++;

        return p;
    }

    void ProxyAllocator::Deallocate(void* ptr)
    {
        size_t memory = m_Allocator.GetUsedMemorySize();

        m_Allocator.Deallocate(ptr);

        m_UsedMemorySize -= memory - m_Allocator.GetUsedMemorySize();
        m_AllocationsCount++;
    }

}}