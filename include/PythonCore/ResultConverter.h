#pragma once

#include "PythonCore/IScript.h"

namespace SteelEngine { namespace Script { namespace Python {

    class ResultConverter
    {
        friend class Scriptable;
    private:
        IScript*    m_Python;
        void*       m_Value;

    public:
        ResultConverter()
        {

        }

        ~ResultConverter()
        {

        }

        size_t ToSizeT()
        {
            size_t res = m_Python->ToSizeT(m_Value);

            m_Python->DecRef(m_Value);

            return res;
        }

        double ToDouble()
        {
            double res = m_Python->ToDouble(m_Value);

            m_Python->DecRef(m_Value);

            return res;
        }

        const char* ToString()
        {
            const char* res = m_Python->ToString(m_Value);

            m_Python->DecRef(m_Value);

            return res;
        }
    };

}}}