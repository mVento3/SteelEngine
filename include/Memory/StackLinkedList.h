#pragma once

namespace SteelEngine { namespace Memory {

    template <typename A>
    class StackLinkedList
    {
    public:
        struct Node
        {
            A       m_Data;
            Node*   m_Next;
        };

    private:
        Node* m_Head;

    public:
        StackLinkedList()
        {

        }

        ~StackLinkedList()
        {

        }

        void Push(Node* node)
        {
            node->m_Next = m_Head;
            m_Head = node;
        }

        Node* Pop()
        {
            Node* top = m_Head;

            m_Head = m_Head->m_Next;

            return top;
        }
    };

}}