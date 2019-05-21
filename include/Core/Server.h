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

#include "RuntimeReflection/Reflection.h"
#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/ReflectionAttributes.h"

#include "Server.Generated.h"

namespace SteelEngine {

    SE_CLASS(SteelEngine::SE_SERVER_INFO = SteelEngine::ServerInfo(1024, 54000))
    class Server : public Interface::INetwork
    {
        GENERATED_BODY
        typedef std::map<SOCKET, std::queue<std::string>> SocketMap;
    private:
        fd_set m_Master;
        SOCKET m_ListeningSocket;
        SocketMap m_PendingCommands;
        ServerInfo m_ServerInfo;

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

        void operator()(const Networking::SendMessageEvent& event) override;
    };
    
}