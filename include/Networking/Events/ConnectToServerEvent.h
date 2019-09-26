#pragma once

#include "string"

namespace SteelEngine { namespace Network {

    struct ConnectToServerEvent
    {
        std::string m_ServerIP;

        ConnectToServerEvent(const std::string& ip) :
            m_ServerIP(ip)
        {

        }
    };

}}