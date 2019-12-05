#pragma once

#include "Memory/Allocator.h"

namespace SteelEngine { namespace Memory {

    class ProxyAllocator : public Allocator
    {
    private:
        Allocator& m_Allocator;

    public:
        ProxyAllocator(Allocator& allocator);
        ~ProxyAllocator();

        void* Allocate(size_t size, Type::uint8 alignment) override;
        void Deallocate(void* ptr) override;
    };

}}