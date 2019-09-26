#pragma once

#include "ws2tcpip.h"

#include "Core/Type.h"

namespace SteelEngine { namespace Network {

    struct INetwork;

    struct ClientInfo
    {
        sockaddr_in         m_Address;
        Network::INetwork*  m_Network;
        const SOCKET        m_Socket;
        bool                m_ConnectedToServer;
        bool                m_Busy;

        ClientInfo(sockaddr_in address, Network::INetwork* network, const SOCKET socket) :
            m_Address(address),
            m_Network(network),
            m_Socket(socket)
        {
            m_ConnectedToServer = false;
            m_Busy = false;
        }

        const char* GetIP()
        {
            return inet_ntoa(m_Address.sin_addr);
        }

        Type::uint32 GetPort()
        {
            return ntohs(m_Address.sin_port);
        }
    };

}}