#pragma once

#include "Memory/FixedVectorAllocator.h"

namespace SteelEngine { namespace Container {

    template <typename A>
    class Vector
    {
    private:
        size_t m_MaxElements;
        size_t m_ElementSize;
        size_t m_ElementsCount;
        Memory::FixedVectorAllocator* m_Allocator;

    public:
        Vector(size_t maxElements, void* start) :
            m_MaxElements(maxElements)
        {
            m_ElementSize = sizeof(A);
            m_ElementsCount = 0;
            m_Allocator =
                new(start) Memory::FixedVectorAllocator(
                    maxElements * m_ElementSize,
                    m_ElementSize,
                    __alignof(A),
                    start
                );
        }

        Vector(size_t maxElements, Memory::Allocator* allocator) :
            m_MaxElements(maxElements)
        {
            m_ElementSize = sizeof(A);
            m_ElementsCount = 0;
            m_Allocator =
                Memory::allocate<Memory::FixedVectorAllocator>(
                    *allocator,
                    allocator,
                    maxElements * m_ElementSize,
                    m_ElementSize,
                    __alignof(A)
                );
        }

        Vector(size_t maxElements) :
            Vector(maxElements, Memory::PointerMath::add(this, sizeof(Vector<A>)))
        {

        }

        ~Vector()
        {

        }

        void PushBack(const A& element)
        {
            assert(m_ElementsCount < m_MaxElements);

            A* next = (A*)m_Allocator->Allocate(m_ElementSize, __alignof(A));

            *next = element;

            m_ElementsCount++;
        }

        void PopBack()
        {
            assert(m_ElementsCount != 0);

            m_Allocator->Deallocate(0);

            m_ElementsCount--;
        }

        const A& At(size_t index)
        {
            Type::uptr address = (Type::uptr)m_Allocator->GetStart() + (m_ElementSize * index);
            A* a = (A*)address;

            return *a;
        }

        A* AtPtr(size_t index)
        {
            Type::uptr address = (Type::uptr)m_Allocator->GetStart() + (m_ElementSize * index);

            return (A*)address;
        }

        void Clear()
        {
            for(size_t i = 0; i < m_ElementsCount; i++)
            {
                PopBack();
            }
        }

        inline bool Empty()
        {
            return m_ElementsCount == 0;
        }

        inline size_t Size()
        {
            return m_ElementsCount;
        }
    };

}}