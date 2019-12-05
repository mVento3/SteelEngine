#pragma once

#include "Memory/Allocator.h"
#include "Memory/PointerMath.h"

namespace SteelEngine { namespace Memory {

    class FixedStackAllocator : public Allocator
    {
    protected:
        size_t m_ElementSize;
        Type::uint8 m_Alignment;

    public:
        FixedStackAllocator(size_t maxSizeInBytes, size_t elementSize, Type::uint8 alignment, void* start);
        FixedStackAllocator(size_t maxSizeInBytes, size_t elementSize, Type::uint8 alignment);
        ~FixedStackAllocator();

        void* Allocate(size_t size, Type::uint8 alignment) override;
        void Deallocate(void* ptr) override;
    };

}}