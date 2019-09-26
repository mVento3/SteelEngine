#pragma once

#include "Networking/INetwork.h"
#include "Networking/ClientThread.h"

#include "Serialization/Serialization.h"

namespace SteelEngine { namespace Utils {

    // The data must be serialized!
    void receiveWholeDataS(
        Network::INetwork* network,
        std::string& receivedData,
        char* buffer,
        SOCKET sock,
        size_t bufferSize
    );

}}