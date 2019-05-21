#pragma once

#include <string>

#include "Type.h"

namespace SteelEngine {

    enum
    {
        SE_FALSE,
        SE_TRUE,
        SE_NOT_IMPLEMENTED
    };

    struct Result
    {
        std::string m_Message;
        Type::uint8 m_Result;

        Result(Type::uint8 result = SE_NOT_IMPLEMENTED, const std::string& message = "") :
            m_Result(result),
            m_Message(message)
        {

        }

        inline bool operator==(const Type::uint8& rhs)
        {
            return m_Result == rhs;
        }

        inline bool operator!=(const Type::uint8& rhs)
        {
            return m_Result != rhs;
        }
    };

}