#pragma once

#include "winsock2.h"

namespace SteelEngine { namespace Network {

    struct ClientDisconnectedEvent
    {
        const SOCKET m_Socket;

        ClientDisconnectedEvent(const SOCKET socket) :
            m_Socket(socket)
        {

        }
    };

}}