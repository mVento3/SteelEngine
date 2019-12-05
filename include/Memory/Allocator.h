#pragma once

#include "stdlib.h"

#include "Core/Type.h"

#include "new"
#include "assert.h"
#include "type_traits"

namespace SteelEngine {

    class MemoryTracker;

    namespace Memory {

    class Allocator
    {
        friend class MemoryTracker;
    public:
		static const Type::uint8 DEFAULT_ALIGNMENT = 8;

    private:
        size_t m_AllocatorID;

    protected:
        size_t m_MemorySize;
        size_t m_UsedMemorySize;
        size_t m_AllocationsCount;
        const void* m_Start;

    public:
        Allocator(size_t size, const void* start);
        virtual ~Allocator();

        virtual void* Allocate(size_t size, Type::uint8 alignment = DEFAULT_ALIGNMENT) = 0;
        virtual void Deallocate(void* ptr = 0) = 0;

        inline size_t GetMemorySize() const { return m_MemorySize; }
        inline size_t GetUsedMemorySize() const { return m_UsedMemorySize; }
        inline const void* GetStart() const { return m_Start; }
        inline size_t GetAllocationsNumber() const { return m_AllocationsCount; }
    };

    template <typename A, typename... Args>
    A* allocate(Allocator& allocator, Args&&... args)
    {
        // Here while allocating memory we want to trace the memory

        // IMemoryTracker* aa = *Reflection::GetType("SteelEngine::Core")->GetMetaData(Core::GlobalSystems::MEMORY_TRACKER)->Convert<IMemoryTracker**>();
        A* result = new(allocator.Allocate(sizeof(A), __alignof(A))) A(std::forward<Args>(args)...);

        if(std::is_base_of<A, Allocator>::value)
        {
            printf("AAAA\n");
        }

        return result;
    }

    template <typename A>
    A* allocate(Allocator& allocator, const A& type)
    {
        return new(allocator.Allocate(sizeof(A), __alignof(A))) A(type);
    }

    void* allocateSize(Allocator& allocator, const size_t& size, const Type::uint8& align = 8);

    template <typename A>
    void deallocate(Allocator& allocator, A* ptr)
    {
        ptr->~A();
        allocator.Deallocate(ptr);
    }

    template <typename A>
    A* allocateArray(Allocator& allocator, size_t size)
    {
        assert(size != 0);

        Type::uint8 headerSize = sizeof(size_t) / sizeof(A);

        if(sizeof(size_t) % sizeof(A) > 0)
        {
            headerSize += 1;
        }

        A* array = ((A*)allocator.Allocate(sizeof(A) * (size + headerSize), __alignof(A))) + headerSize;

        *(((size_t*)array) - 1) = size;

        for(size_t i = 0; i < size; i++)
        {
            new(&array[i]) A;
        }

        return array;
    }

    template <typename A>
    void deallocateArray(Allocator& allocator, A* array)
    {
        assert(array != 0);

        size_t size = *(((size_t*)array) - 1);

        for(size_t i = 0; i < size; i++)
        {
			array[i].~A();
        }

        Type::uint8 headerSize = sizeof(size_t) / sizeof(A);

        if(sizeof(size_t) % sizeof(A) > 0)
        {
			headerSize += 1;
        }

        allocator.Deallocate(array - headerSize);
    }

}}