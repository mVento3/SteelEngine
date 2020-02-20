#pragma once

#include "RuntimeReflection/Macro.h"

#include "Networking/INetworkManager.h"
#include "Networking/INetwork.h"
#include "Networking/GetNetworkInfo.h"

#include "Networking/Events/ServerDisconnectedEvent.h"
#include "Networking/Events/ClientConnectedEvent.h"
#include "Networking/Events/ClientDisconnectedEvent.h"
#include "Networking/Events/HostServerEvent.h"
#include "Networking/Events/ConnectToServerEvent.h"

#include "vector"

#include "Networking/NetworkManager.Generated.h"

namespace SteelEngine { namespace Network {

    SE_CLASS()
    class NetworkManager : public INetworkManager
    {
        GENERATED_BODY
    public:
        typedef std::vector<ClientInfo> ClientInfoVector;

    private:
        INetwork* m_Network;
        ClientInfoVector m_ConnectedClients;
        std::vector<HotReloader::InheritanceTrackKeeper*> m_CommandTypes;
        bool* m_Connected;

    public:
        NetworkManager();
        ~NetworkManager();

        void Init() override;

        const std::vector<HotReloader::InheritanceTrackKeeper*>& GetCommands() override { return m_CommandTypes; }

        template <typename ...Args>
        static void CallNetworkCommand(const char* commandName, const Args&... args)
        {
            Network::INetworkCommand* ev = (Network::INetworkCommand*)Reflection::CreateInstance(commandName, args...);

            Event::GlobalEvent::Broadcast_(ev);

            Reflection::DestroyInstance(ev);
        }

        SE_METHOD()
        Type::uint32 GetConnectedClientsCount();

        SE_METHOD()
        inline bool* GetConnectionStatus() { return m_Connected; }

        void operator()(GetNetworkInfo* event);
        void operator()(const Network::ServerDisconnectedEvent& event);
        void operator()(const Network::ClientConnectedEvent& event);
        void operator()(const Network::ClientDisconnectedEvent& event);

        void operator()(const HostServerEvent& event);
        void operator()(const ConnectToServerEvent& event);
    };

}}