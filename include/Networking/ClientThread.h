#pragma once

#include "thread"
#include "ws2tcpip.h"
#include "future"

namespace SteelEngine { namespace Network {

    struct ClientThread
    {
        std::thread         m_Thread;
        std::promise<void>  m_Signal;
        std::future<void>   m_FutureObject;

        char*           m_Buffer;
        const SOCKET    m_Socket;

        ClientThread(const SOCKET& sock) :
            m_Socket(sock)
        {
            m_FutureObject = m_Signal.get_future();
        }
    };

}}