#pragma once

#include "Networking/INetworkCommand.h"

namespace SteelEngine { namespace NetworkCommands {

    SE_CLASS(SteelEngine::ReflectionAttribute::SE_NO_SERIALIZE, SteelEngine::ReflectionAttribute::SE_NETWORK_COMMAND)
    struct NoneNetCommandEvent : public INetworkCommand
    {
        NoneNetCommandEvent()
        {
            strcpy(m_Header, SE_GET_TYPE_NAME(NoneNetCommandEvent));
        }

        SE_METHOD()
        char* Serialize(char* data) override
        {
            return INetworkCommand::Serialize(data);
        }

        SE_METHOD()
        char* Deserialize(char* data) override
        {
            return INetworkCommand::Deserialize(data);
        }

        SE_METHOD()
        void ServerSide(Interface::INetwork* network, SOCKET sock) override
        {

        }

        SE_METHOD()
        void ClientSide(Interface::INetwork* network, SOCKET sock) override
        {

        }
    };

}}