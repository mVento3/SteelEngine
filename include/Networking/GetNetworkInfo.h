#pragma once

#include "Networking/INetwork.h"

namespace SteelEngine {

    struct GetNetworkInfo
    {
        const Network::INetwork* m_Network;
    };

}