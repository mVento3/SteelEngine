#pragma once

#include "typeinfo"

#include "RuntimeDatabase/RuntimeDatabase.h"

#define SE_EVENT(type) \
    type() \
    { \
        m_Type = typeid(type).hash_code(); \
    } \
    static size_t GetStaticType() { return typeid(type).hash_code(); }

namespace SteelEngine { namespace Event {

    class Naive
    {
    protected:
        size_t m_Type;
        bool m_Handled;

    public:
        Naive()
        {
            m_Type = RuntimeDatabase::s_InvalidID;
            m_Handled = false;
        }

        ~Naive()
        {

        }

        size_t GetType() { return m_Type; }
    };

}}