#pragma once

#include "Memory/FixedStackAllocator.h"
#include "Memory/PointerMath.h"

namespace SteelEngine { namespace Container {

    template <typename A>
    class Stack
    {
    private:
        size_t m_MaxElements;
        size_t m_ElementSize;
        size_t m_ElementsCount;
        Memory::Allocator* m_Allocator;

    public:
        Stack(size_t maxElements, void* start) :
            m_MaxElements(maxElements)
        {
            m_ElementSize = sizeof(A);
            m_ElementsCount = 0;
            m_Allocator =
                new(start) Memory::FixedStackAllocator(
                    maxElements * m_ElementSize,
                    m_ElementSize,
                    __alignof(A),
                    start
                );
        }

        Stack(size_t maxElements, Memory::Allocator* allocator) :
            m_MaxElements(maxElements)
        {
            m_ElementSize = sizeof(A);
            m_ElementsCount = 0;
            m_Allocator =
                Memory::allocate<Memory::FixedStackAllocator>(
                    *allocator,
                    allocator,
                    maxElements * m_ElementSize,
                    m_ElementSize,
                    __alignof(A)
                );
        }

        Stack(size_t maxElements) :
            Stack(maxElements, Memory::PointerMath::add(this, sizeof(Stack<A>)))
        {

        }

        ~Stack()
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

            m_Allocator->Deallocate();

            m_ElementsCount--;
        }

        inline bool Empty()
        {
            return m_ElementsCount == 0;
        }

        inline size_t Size()
        {
            return m_ElementsCount;
        }

        A Top()
        {
            assert(m_ElementsCount != 0);

            Type::uptr current = (Type::uptr)m_Allocator->GetStart() + (m_ElementsCount - 1) * m_ElementSize;
            Type::uint8 adjustment = Memory::PointerMath::calculateForwardAdjustment(current, __alignof(A));

            return *(A*)(current + adjustment);
        }

        inline Memory::Allocator* GetAllocator() const { return m_Allocator; }
    };

}}