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
        SteelEngine::SERVER_INFO = SteelEngine::ServerInfo(1024, 5004)
    )
    class Server : public Network::INetwork
    {
        GENERATED_BODY
    public:
        typedef std::map<SOCKET, std::queue<Network::INetworkCommand*>> SocketMap;

    private:
        SOCKET      m_ListeningSocket;
        Variant*    m_ServerInfo;
        SocketMap   m_Commands;
        std::string m_Header;

        fd_set m_ReadFDS;
        int m_MaxSD;
        static constexpr uint32_t MAX_CLIENTS = 32;
        int m_SD;
        int m_Clients[MAX_CLIENTS];
        int m_Activity;
        int m_NewConnection;
        sockaddr_in hint;
        int hintLen;
        char m_Buffer[1024];

        std::thread* m_WaitForConnectionThread;

        Network::INetworkManager* m_NetworkManager;

        Event::LocalEvent<Network::ClientDisconnectedEvent> m_Disconnected;

        void ProcessClient();

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