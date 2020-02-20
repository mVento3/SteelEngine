#pragma once

#include "vector"
#include "queue"
#include "fstream"
#include "map"
#include "ws2tcpip.h"

#ifdef SE_LINUX
#include "unistd.h"
#endif

#include "Networking/Config.h"
#include "Networking/INetwork.h"
#include "Networking/ClientInfo.h"
#include "Networking/Server.Generated.h"
#include "Networking/INetworkCommand.h"
#include "Networking/ClientThread.h"

#include "RuntimeReflection/Reflection.h"
#include "RuntimeReflection/Macro.h"

#include "Event/LocalEvent.h"

namespace SteelEngine {

    SE_CLASS(
        SteelEngine::SERVER_INFO = SteelEngine::ServerInfo(1024, 5004),
        Reflection::ReflectionAttribute::HOT_RELOAD
    )
    class Server : public Network::INetwork
    {
        GENERATED_BODY
    public:
        typedef std::map<SOCKET, std::queue<Network::INetworkCommand*>> SocketMap;
        typedef std::vector<Network::ClientThread*> ClientThreadVector;

    private:
        SOCKET      m_ListeningSocket;
        Variant*    m_ServerInfo;
        SocketMap   m_Commands;
        std::string m_Header;

        ClientThreadVector m_ClientsThreads;

        std::thread* m_WaitForConnectionThread;

        Network::INetworkManager* m_NetworkManager;

        Event::LocalEvent<Network::ClientDisconnectedEvent> m_Disconnected;

        void ProcessClient(Network::ClientThread* info);

    public:
        Server();
        ~Server();

        void Update() override;

        SE_METHOD()
        void Start();

        SE_METHOD()
        int Send(SOCKET sock, const char* buffer, Type::uint32 size) override;

        SE_METHOD()
        int Receive(SOCKET sock, char* buffer, Type::uint32 size) override;

        SE_METHOD()
        std::queue<Network::INetworkCommand*>* GetCommands(SOCKET sock);

        void SetNetworkManager(Network::INetworkManager* networkManager) override { m_NetworkManager = networkManager; }

        void operator()(Network::INetworkCommand* event);
        void operator()(const Network::ClientDisconnectedEvent& event);
    };

}