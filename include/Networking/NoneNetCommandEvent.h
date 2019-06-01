#pragma once

#include "Networking/INetworkCommand.h"

#include "NoneNetCommandEvent.Generated.h"

namespace SteelEngine { namespace NetworkCommands {

    SE_CLASS(SteelEngine::ReflectionAttribute::SE_NO_SERIALIZE, SteelEngine::ReflectionAttribute::SE_NETWORK_COMMAND)
    struct NoneNetCommandEvent : public INetworkCommand
    {
        GENERATED_BODY

        NoneNetCommandEvent()
        {
            m_Header = SE_GET_TYPE_NAME(NoneNetCommandEvent);
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