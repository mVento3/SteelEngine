#pragma once

#include "Networking/ClientInfo.h"

namespace SteelEngine { namespace Network {

    struct ClientConnectedEvent
    {
        ClientInfo m_ClientInfo;
    };

}}