#pragma once

#include "Networking/INetworkCommand.h"
#include "Networking/Config.h"

#include "string"
#include "fstream"
#include "queue"

#include "Core/ReflectionAttributes.h"

#include "Networking/Commands/SwapModuleNetCommandEvent.Generated.h"

namespace SteelEngine { namespace Network {

    SE_CLASS(SteelEngine::ReflectionAttribute::NO_SERIALIZE = true, SteelEngine::ReflectionAttribute::NETWORK_COMMAND)
    class SwapModuleNetCommandEvent : public INetworkCommand
    {
        GENERATED_BODY
    public:
        SwapModuleNetCommandEvent();
        SwapModuleNetCommandEvent(const char* moduleName);

        SE_VALUE(SteelEngine::ReflectionAttribute::NET_VALUE)
        std::string m_ModuleName = "";

        char* m_Buffer;

        SE_METHOD()
        void ServerSide(Network::INetwork* network, SOCKET sock) override;

        SE_METHOD()
        void ClientSide(Network::INetwork* network, SOCKET sock) override;
    };

}}