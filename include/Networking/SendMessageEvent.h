#pragma once

#include "string"

namespace SteelEngine { namespace Networking {

    struct SendMessageEvent
    {
        std::string m_Command;
        size_t m_Size = 0;
    };

}}