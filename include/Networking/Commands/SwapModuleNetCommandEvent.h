#pragma once

#include "Networking/INetworkCommand.h"
#include "Networking/Config.h"

#include "string"
#include "fstream"
#include "queue"

#include "Networking/Commands/SwapModuleNetCommandEvent.Generated.h"

namespace SteelEngine { namespace Network {

    SE_CLASS(
        SteelEngine::Reflection::ReflectionAttribute::NO_SERIALIZE,
        SteelEngine::Reflection::ReflectionAttribute::NETWORK_COMMAND,
        SteelEngine::Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS,
        Reflection::ReflectionAttribute::HOT_RELOAD
    )
    class SwapModuleNetCommandEvent :
        public INetworkCommand,
        public HotReloader::IRuntimeObject
    {
        GENERATED_BODY
    public:
        SwapModuleNetCommandEvent();
        SwapModuleNetCommandEvent(const char* moduleName);

        SE_VALUE(SteelEngine::Reflection::ReflectionAttribute::NET_VALUE)
        std::string m_ModuleName = "";

        char* m_Buffer;

        SE_METHOD()
        void ServerSide(Network::INetwork* network, SOCKET sock) override;

        SE_METHOD()
        void ClientSide(Network::INetwork* network, SOCKET sock) override;
    };

}}