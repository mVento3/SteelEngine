#pragma once

#include "Memory/Iterator.h"
#include "Memory/Allocator.h"
#include "Memory/StackLinkedList.h"

namespace SteelEngine { namespace Memory {

    template <typename A>
    class PoolAllocator : public Allocator
    {
    public:
        using Node = typename StackLinkedList<A>::Node;

    private:
        class PoolIterator
        {
        private:
            PoolAllocator::Node* m_Node;

        public:
            PoolIterator(PoolAllocator::Node* node) :
                m_Node(node)
            {

            }

            ~PoolIterator()
            {

            }

            PoolIterator operator++(int)
            {
                Iterator original(*this);

                m_Node = m_Node->m_Next;

                return original;
            }

            PoolIterator& operator++()
            {
                m_Node = m_Node->m_Next;

                return *this;
            }

            A* operator->() const
            {
                return (A*)&m_Node->m_Data;
            }

            // Reference operator*() const
            // {
            //     return *(Pointer)&m_Node->m_Data;
            // }

            A* operator&() const
            {
                return (A*)&m_Node->m_Data;
            }
        };

    public:
        using Iterator = PoolIterator;

    protected:
        size_t m_ChunkSize;
        StackLinkedList<A> m_FreeList;
        Node* m_Last;

    public:
        PoolAllocator(void* startAddress, size_t count) :
            Allocator(count * sizeof(Node), PointerMath::add(startAddress, sizeof(PoolAllocator))),
            m_ChunkSize(sizeof(Node))
        {
            Reset();
        }

        PoolAllocator(size_t count) :
            Allocator(count * sizeof(Node), PointerMath::add(this, sizeof(PoolAllocator))),
            m_ChunkSize(sizeof(Node))
        {
            Reset();
        }

        PoolAllocator(size_t count, Allocator* allocator) :
            Allocator(count * sizeof(Node), PointerMath::add(allocator->Allocate(count * sizeof(Node) + sizeof(PoolAllocator), __alignof(PoolAllocator)), sizeof(PoolAllocator))),
            m_ChunkSize(sizeof(Node))
        {
            Reset();
        }

        ~PoolAllocator()
        {

        }

        void* Allocate(size_t size, Type::uint8 alignment) override
        {
            assert(size <= m_ChunkSize && "The size is too big!");

            Node* freeAddress = m_FreeList.Pop();

            m_UsedMemorySize += m_ChunkSize;
            m_AllocationsCount++;

            freeAddress->m_Next = m_Last;
            m_Last = freeAddress;

            return &freeAddress->m_Data;
        }

        void Deallocate(void* ptr) override
        {
            m_FreeList.Push((Node*)ptr);

            m_UsedMemorySize -= m_ChunkSize;
            m_AllocationsCount--;
        }

        void Reset()
        {
            m_UsedMemorySize = 0;
            m_AllocationsCount = 0;
            m_Last = 0;

            const size_t nChunks = m_MemorySize / m_ChunkSize;

            for(size_t i = 0; i < nChunks; i++)
            {
                Type::uptr address = (Type::uptr)m_Start + i * m_ChunkSize;

                m_FreeList.Push((Node*)address);
            }
        }

        inline Iterator Begin() { return Iterator((Node*)((Type::uptr)m_Start + m_MemorySize - m_UsedMemorySize)); }
        inline Iterator End() { return Iterator((Node*)this); }
    };

}}