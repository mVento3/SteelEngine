#pragma once

#include "winsock2.h"

#include "HotReloader/IRuntimeObject.h"

#include "Event/LocalEvent.h"

#include "Networking/Events/ServerDisconnectedEvent.h"
#include "Networking/Events/ClientConnectedEvent.h"
#include "Networking/Events/ClientDisconnectedEvent.h"

namespace SteelEngine { namespace Network {

    struct INetworkManager;

    struct INetwork
    {
        Event::LocalEvent<ServerDisconnectedEvent>  m_DisconnectEvent;
        Event::LocalEvent<ClientConnectedEvent>     m_ClientConnectedEvent;
        Event::LocalEvent<ClientDisconnectedEvent>  m_ClientDisconnectedEvent;

        virtual void Update() { }

        virtual int Receive(SOCKET sock, char* buffer, Type::uint32 size) = 0;
        virtual int Send(SOCKET sock, const char* buffer, Type::uint32 size) = 0;

        virtual void SetNetworkManager(INetworkManager* networkManager) = 0;
    };

}}