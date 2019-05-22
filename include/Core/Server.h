#pragma once

#include "vector"
#include "thread"
#include "queue"
#include "fstream"
#include "map"

#ifndef SE_WINDOWS
#include "unistd.h"
#endif

#include "Networking/Config.h"
#include "Networking/INetwork.h"
#include "Networking/NetworkCommands.h"

#include "RuntimeReflection/Reflection.h"
#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/ReflectionAttributes.h"

#include "Server.Generated.h"

#include "Event/localevent.h"

namespace SteelEngine {

    SE_CLASS(SteelEngine::SE_SERVER_INFO = SteelEngine::ServerInfo(1024, 54000))
    class Server : public Interface::INetwork
    {
        GENERATED_BODY
    public:
        struct MessageData
        {
            char* m_Data;
            size_t m_Size;
        };

        typedef std::map<SOCKET, std::queue<MessageData>> SocketMap2;

    private:
        fd_set m_Master;
        SOCKET m_ListeningSocket;
        ServerInfo m_ServerInfo;
        SocketMap2 m_Commands;

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

        void operator()(NetworkCommands::INetworkCommand* event);
    };
    
}