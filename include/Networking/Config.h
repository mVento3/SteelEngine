#pragma once

#include "Core/Type.h"

namespace SteelEngine {

    enum
    {
        SE_SERVER_INFO
    };

    struct ServerInfo
    {
        Type::uint32 m_BufferSize;
        Type::uint32 m_Port;

        ServerInfo(Type::uint32 bufferSize = 512, Type::uint32 port = 54000) :
            m_BufferSize(bufferSize),
            m_Port(port)
        {

        }
    };

}