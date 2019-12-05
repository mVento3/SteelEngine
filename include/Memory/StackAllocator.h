#pragma once

#include "Memory/Allocator.h"
#include "Memory/PointerMath.h"

namespace SteelEngine { namespace Memory {

    class StackAllocator : public Allocator
    {
    public:
        struct Header
        {
            size_t m_Size;
            size_t m_Padding;
        };

    private:
        Type::uptr m_PaddingEndAddress;

        Allocator* m_Allocator;

    public:
        StackAllocator(size_t size, void* start);
        StackAllocator(Allocator* allocator, size_t size);
        ~StackAllocator();

        void* Allocate(size_t size, Type::uint8 alignment) override;
        void Deallocate(void* ptr) override;

        void Reset();
    };

}}