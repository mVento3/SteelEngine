#include "Memory/Allocator.h"

#include "assert.h"

namespace SteelEngine { namespace Memory {

    Allocator::Allocator(size_t size, const void* start) :
        m_MemorySize(size),
        m_Start(start)
    {
        m_UsedMemorySize = 0;
        m_AllocationsCount = 0;
    }

    Allocator::~Allocator()
    {
        assert(m_AllocationsCount == 0 && m_UsedMemorySize == 0);
    }

    void* allocateSize(Allocator& allocator, const size_t& size, const Type::uint8& align)
    {
        return allocator.Allocate(size, align);
    }

}}