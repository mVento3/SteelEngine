#pragma once

#include "Memory/Allocator.h"
#include "Memory/PointerMath.h"

namespace SteelEngine { namespace Memory {

    class FixedVectorAllocator : public Allocator
    {
    private:
        size_t m_ElementSize;
        Type::uint8 m_Alignment;

    public:
        FixedVectorAllocator(size_t maxSizeInBytes, size_t elementSize, Type::uint8 alignment, void* start);
        FixedVectorAllocator(size_t maxSizeInBytes, size_t elementSize, Type::uint8 alignment);
        FixedVectorAllocator(Allocator* allocator, size_t maxSizeInBytes, size_t elementSize, Type::uint8 alignment);
        ~FixedVectorAllocator();

        void* Allocate(size_t size, Type::uint8 alignment) override;
        void Deallocate(void* ptr) override;
    };

}}