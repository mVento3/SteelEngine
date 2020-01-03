#include "Memory/StackAllocator.h"

#include "assert.h"

namespace SteelEngine { namespace Memory {

    StackAllocator::StackAllocator(size_t size, void* start) :
        Allocator(size, start)
    {

    }

    StackAllocator::StackAllocator(Allocator* allocator, size_t size) :
        Allocator(size, allocator->Allocate(size + sizeof(StackAllocator), __alignof(StackAllocator))),
        m_Allocator(allocator)
    {

    }

    StackAllocator::~StackAllocator()
    {

    }

    void* StackAllocator::Allocate(size_t size, Type::uint8 alignment)
    {
        assert(size <= m_MemorySize && "The size is too big!");

        const Type::uptr currentAddress = (Type::uptr)m_Start + m_UsedMemorySize;
        Type::uptr padding = PointerMath::calculatePaddingWithHeader<Header>(currentAddress, alignment);

        assert(m_UsedMemorySize + padding + size <= m_MemorySize && "Overflow!");

        Type::uptr nextAddress = currentAddress + padding;
        Type::uptr headerAddress = nextAddress - sizeof(Header);
        Header* header = (Header*)headerAddress;

        header->m_Size = size;
        header->m_Padding = padding;

        m_UsedMemorySize += padding + size;
        m_AllocationsCount++;
        m_PaddingEndAddress = nextAddress;

        void* next = (void*)nextAddress;

        return (void*)nextAddress;
    }

    void StackAllocator::Deallocate(void* ptr)
    {
        const Type::uptr headerAddress = m_PaddingEndAddress - sizeof(Header);
        const Header* header = (Header*)headerAddress;
        unsigned char* start = (unsigned char*)headerAddress;
        size_t size = header->m_Size;
        size_t padding = header->m_Padding;
        void* addr = (void*)m_PaddingEndAddress;

        for(size_t i = 0; i < sizeof(Header) + size; i++)
        {
            *start = '\0';
            start++;
        }

        m_UsedMemorySize -= padding;
        m_UsedMemorySize -= size;

        m_PaddingEndAddress -= padding;
        m_PaddingEndAddress -= size;
    }

    void StackAllocator::Reset()
    {
        m_UsedMemorySize = 0;
        m_AllocationsCount = 0;
    }

}}