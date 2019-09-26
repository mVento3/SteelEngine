#pragma once

#include "RuntimeCompiler/IRuntimeObject.h"

#include "Networking/ClientInfo.h"
#include "Networking/INetworkCommand.h"

namespace SteelEngine { namespace Network {

    struct INetworkManager : public HotReload::IRuntimeObject
    {
        virtual void Init() = 0;

        virtual const std::vector<INetworkCommand*>& GetCommands() = 0;
    };

}}