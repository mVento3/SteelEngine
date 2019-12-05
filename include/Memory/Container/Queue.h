#pragma once

#include "Memory/FixedQueueAllocator.h"

namespace SteelEngine { namespace Container {

    template <typename A>
    class Queue
    {
    private:
        size_t m_MaxElements;
        size_t m_ElementSize;
        size_t m_ElementsCount;
        Memory::FixedQueueAllocator* m_Allocator;

    public:
        Queue(size_t maxElements, void* start) :
            m_MaxElements(maxElements)
        {
            m_ElementSize = sizeof(A);
            m_ElementsCount = 0;
            m_Allocator =
                new(start) Memory::FixedQueueAllocator(
                    maxElements * m_ElementSize,
                    m_ElementSize,
                    __alignof(A),
                    start
                );
        }

        Queue(size_t maxElements, Memory::Allocator* allocator) :
            m_MaxElements(maxElements)
        {
            m_ElementSize = sizeof(A);
            m_ElementsCount = 0;
            m_Allocator =
                Memory::allocate<Memory::FixedQueueAllocator>(
                    *allocator,
                    maxElements * m_ElementSize,
                    m_ElementSize,
                    __alignof(A)
                );
        }

        Queue(size_t maxElements) :
            Queue(maxElements, Memory::PointerMath::add(this, sizeof(Queue<A>)))
        {

        }

        ~Queue()
        {

        }

        void Push(const A& element)
        {
            assert(m_ElementsCount < m_MaxElements);

            A* next = (A*)m_Allocator->Allocate(m_ElementSize, __alignof(A));

            *next = element;

            m_ElementsCount++;
        }

        void Pop()
        {
            assert(m_ElementsCount != 0);

            m_Allocator->Deallocate(0);

            m_ElementsCount--;
        }

        A Front()
        {
            assert(m_ElementsCount != 0);

            return *(A*)((Type::uptr)m_Allocator->GetStart() + m_Allocator->GetFrontPosition());
        }

        A Back()
        {
            assert(m_ElementsCount != 0);

            return *(A*)((Type::uptr)m_Allocator->GetStart() + m_Allocator->GetRearPosition());
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