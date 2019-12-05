#pragma once

#include "Memory/Allocator.h"
#include "Memory/PointerMath.h"

namespace SteelEngine { namespace Memory {

    class LinearAllocator : public Allocator
    {
    private:

    public:
        LinearAllocator(size_t size, void* start);
        LinearAllocator(Allocator* allocator, size_t size);
        LinearAllocator(size_t size);
        ~LinearAllocator();

        void* Allocate(size_t size, Type::uint8 alignment) override;
        void Deallocate(void* ptr) override;
    };

}}