#include "Memory/FixedQueueAllocator.h"

#include "assert.h"
#include "limits"

namespace SteelEngine { namespace Memory {

    FixedQueueAllocator::FixedQueueAllocator(size_t maxSizeInBytes, size_t elementSize, Type::uint8 alignment, void* start) :
        Allocator(maxSizeInBytes, Memory::PointerMath::calculateForwardAddress((Type::uptr)PointerMath::add(start, sizeof(FixedQueueAllocator)), alignment)),
        m_ElementSize(elementSize),
        m_Alignment(alignment)
    {
        m_RearPosition = ULLONG_MAX;
        m_FrontPosition = ULLONG_MAX;
    }

    FixedQueueAllocator::FixedQueueAllocator(size_t maxSizeInBytes, size_t elementSize, Type::uint8 alignment) :
        Allocator(maxSizeInBytes, Memory::PointerMath::calculateForwardAddress((Type::uptr)PointerMath::add(this, sizeof(FixedQueueAllocator)), alignment)),
        m_ElementSize(elementSize),
        m_Alignment(alignment)
    {
        m_RearPosition = ULLONG_MAX;
        m_FrontPosition = ULLONG_MAX;
    }

    FixedQueueAllocator::FixedQueueAllocator(Allocator* allocator, size_t maxSizeInBytes, size_t elementSize, Type::uint8 alignment) :
        Allocator(
            maxSizeInBytes,
            Memory::PointerMath::calculateForwardAddress(
                (Type::uptr)PointerMath::add(allocator->Allocate(maxSizeInBytes + sizeof(FixedQueueAllocator), __alignof(FixedQueueAllocator)), sizeof(FixedQueueAllocator)),
                alignment
            )
        ),
        m_ElementSize(elementSize),
        m_Alignment(alignment)
    {
        m_RearPosition = ULLONG_MAX;
        m_FrontPosition = ULLONG_MAX;
    }

    FixedQueueAllocator::~FixedQueueAllocator()
    {

    }

    void* FixedQueueAllocator::Allocate(size_t size, Type::uint8 alignment)
    {
        assert(size == m_ElementSize);
        assert(alignment == m_Alignment);
        assert(m_UsedMemorySize < m_MemorySize);

        Type::uptr next = 0;

        // Type::uptr next = (Type::uptr)m_Start + m_UsedMemorySize + m_Position;

        // m_UsedMemorySize += m_ElementSize;

        if(m_FrontPosition == ULLONG_MAX)
        {
            m_FrontPosition = m_RearPosition = 0;
            next = (Type::uptr)m_Start + m_RearPosition;
        }
        else if(m_RearPosition == m_MemorySize - m_ElementSize && m_FrontPosition != 0)
        {
            m_RearPosition = 0;
            next = (Type::uptr)m_Start + m_RearPosition;
        }
        else
        {
            m_RearPosition += m_ElementSize;
            next = (Type::uptr)m_Start + m_RearPosition;
        }

        m_UsedMemorySize += m_ElementSize;

        return (void*)next;
    }

    void FixedQueueAllocator::Deallocate(void* ptr)
    {
        assert(m_UsedMemorySize <= m_MemorySize);

        Type::uptr current = (Type::uptr)m_Start + m_FrontPosition;

        unsigned char* toClearPtr = (unsigned char*)current;

        for(size_t i = 0; i < m_ElementSize; i++)
        {
            *toClearPtr = '\0';

            toClearPtr++;
        }

        if(m_FrontPosition == m_RearPosition)
        {
            m_FrontPosition = 0;
            m_RearPosition = 0;
        }
        else if(m_FrontPosition == m_MemorySize - m_ElementSize)
        {
            m_FrontPosition = 0;
        }
        else
        {
            m_FrontPosition += m_ElementSize;
        }

        m_UsedMemorySize -= m_ElementSize;

        // Type::uptr current = (Type::uptr)m_Start + m_Position;
        // unsigned char* toClearPtr = (unsigned char*)current;

        // for(size_t i = 0; i < m_ElementSize; i++)
        // {
        //     *toClearPtr = '\0';

        //     toClearPtr++;
        // }

        // m_UsedMemorySize -= m_ElementSize;
        // m_Position += m_ElementSize;
    }

}}