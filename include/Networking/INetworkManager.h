#pragma once

#include "HotReloader/IRuntimeObject.h"
#include "HotReloader/InheritanceTrackKeeper.h"

#include "Networking/ClientInfo.h"
#include "Networking/INetworkCommand.h"

namespace SteelEngine { namespace Network {

    struct INetworkManager
    {
        virtual void Init() = 0;

        virtual const std::vector<HotReloader::InheritanceTrackKeeper*>& GetCommands() = 0;
    };

}}