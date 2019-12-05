#include "Memory/LinearAllocator.h"

#include "assert.h"

namespace SteelEngine { namespace Memory {

    LinearAllocator::LinearAllocator(size_t size, void* start) :
        Allocator(size, start)
    {

    }

    LinearAllocator::LinearAllocator(Allocator* allocator, size_t size) :
        Allocator(size, allocator->Allocate(size, __alignof(LinearAllocator)))
    {

    }

    LinearAllocator::LinearAllocator(size_t size) :
        Allocator(size, PointerMath::add(this, sizeof(LinearAllocator)))
    {

    }

    LinearAllocator::~LinearAllocator()
    {

    }

    void* LinearAllocator::Allocate(size_t size, Type::uint8 alignment)
    {
        const Type::uptr currentAddress = (Type::uptr)m_Start + m_UsedMemorySize;
        Type::uint8 adjustment = PointerMath::calculateForwardAdjustment(currentAddress, alignment);

        assert(m_UsedMemorySize + adjustment + size <= m_MemorySize && "Overflow!");

        const Type::uptr nextAddress = currentAddress + adjustment;

        m_AllocationsCount++;
        m_UsedMemorySize += size + adjustment;

        return (void*)nextAddress;
    }

    void LinearAllocator::Deallocate(void* ptr)
    {
        unsigned char* start = (unsigned char*)m_Start;

        for(size_t i = 0; i < m_UsedMemorySize; i++)
        {
            *start = '\0';
            start++;
        }

        m_AllocationsCount = 0;
        m_UsedMemorySize = 0;
    }

}}