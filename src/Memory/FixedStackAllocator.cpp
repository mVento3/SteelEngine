#include "Memory/FixedStackAllocator.h"

#include "assert.h"

namespace SteelEngine { namespace Memory {

    FixedStackAllocator::FixedStackAllocator(size_t maxSizeInBytes, size_t elementSize, Type::uint8 alignment, void* start) :
        Allocator(maxSizeInBytes, Memory::PointerMath::calculateForwardAddress((Type::uptr)PointerMath::add(start, sizeof(FixedStackAllocator)), alignment)),
        m_ElementSize(elementSize),
        m_Alignment(alignment)
    {

    }

    FixedStackAllocator::FixedStackAllocator(size_t maxSizeInBytes, size_t elementSize, Type::uint8 alignment) :
        Allocator(maxSizeInBytes, Memory::PointerMath::calculateForwardAddress((Type::uptr)PointerMath::add(this, sizeof(FixedStackAllocator)), alignment)),
        m_ElementSize(elementSize),
        m_Alignment(alignment)
    {

    }

    FixedStackAllocator::FixedStackAllocator(Allocator* allocator, size_t maxSizeInBytes, size_t elementSize, Type::uint8 alignment) :
        Allocator(
            maxSizeInBytes,
            Memory::PointerMath::calculateForwardAddress(
                (Type::uptr)PointerMath::add(allocator->Allocate(maxSizeInBytes + maxSizeInBytes, __alignof(FixedStackAllocator)), sizeof(FixedStackAllocator)),
                alignment
            )
        ),
        m_ElementSize(elementSize),
        m_Alignment(alignment)
    {

    }

    FixedStackAllocator::~FixedStackAllocator()
    {

    }

    void* FixedStackAllocator::Allocate(size_t size, Type::uint8 alignment)
    {
        assert(size == m_ElementSize);
        assert(alignment == m_Alignment);

        Type::uptr next = (Type::uptr)m_Start + m_UsedMemorySize;

        m_UsedMemorySize += m_ElementSize;
        m_AllocationsCount++;

        return (void*)next;
    }

    void FixedStackAllocator::Deallocate(void* ptr)
    {
        assert(m_UsedMemorySize <= m_MemorySize);

        Type::uptr current = (Type::uptr)m_Start + (m_UsedMemorySize - m_ElementSize);
        unsigned char* toClearPtr = (unsigned char*)current;

        for(size_t i = 0; i < m_ElementSize; i++)
        {
            *toClearPtr = '\0';

            toClearPtr++;
        }

        m_UsedMemorySize -= m_ElementSize;
        m_AllocationsCount--;
    }

}}