#pragma once

namespace SteelEngine { namespace Memory {

    template <typename A>
    class Iterator
    {
    public:
        typedef A* Pointer;
        typedef A& Reference;
        typedef A Type;

    private:
        A* m_Position;

    public:
        Iterator()
        {
            m_Position = 0;
        }

        Iterator(A* begin) :
            m_Position(begin)
        {

        }

        ~Iterator()
        {

        }

        virtual Iterator operator++(int)
        {
            return m_Position++;
        }

        virtual Iterator& operator++()
        {
            ++m_Position;

            return *this;
        }

        virtual Pointer operator->() const
        {
            return m_Position;
        }

        virtual Reference operator*() const
        {
            return *m_Position;
        }

        virtual Pointer operator&() const
        {
            return m_Position;
        }

        virtual bool operator==(const Iterator& rhs)
        {
            return m_Position == rhs.m_Position;
        }

        virtual bool operator!=(const Iterator& rhs)
        {
            return m_Position != rhs.m_Position;
        }
    };

}}