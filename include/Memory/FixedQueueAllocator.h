#pragma once

#include "Memory/Allocator.h"
#include "Memory/PointerMath.h"

namespace SteelEngine { namespace Memory {

    class FixedQueueAllocator : public Allocator
    {
    private:
        size_t m_ElementSize;
        Type::uint8 m_Alignment;
        Type::uptr m_RearPosition;
        Type::uptr m_FrontPosition;

    public:
        FixedQueueAllocator(size_t maxSizeInBytes, size_t elementSize, Type::uint8 alignment, void* start);
        FixedQueueAllocator(size_t maxSizeInBytes, size_t elementSize, Type::uint8 alignment);
        FixedQueueAllocator(Allocator* allocator, size_t maxSizeInBytes, size_t elementSize, Type::uint8 alignment);
        ~FixedQueueAllocator();

        void* Allocate(size_t size, Type::uint8 alignment) override;
        void Deallocate(void* ptr) override;

        inline const Type::uptr& GetRearPosition() const { return m_RearPosition; }
        inline const Type::uptr& GetFrontPosition() const { return m_FrontPosition; }
    };

}}