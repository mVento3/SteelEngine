#pragma once

#include "Networking/INetwork.h"
#include "Networking/Config.h"

#include "Networking/GetNetCommandEvent.h"

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/Reflection.h"
#include "RuntimeReflection/ReflectionAttributes.h"

#include "Client.Generated.h"

#include "Core/Result.h"

#include "ws2tcpip.h"
#include "thread"
#include "queue"
#include "string"

#include "Event/globalevent.h"

#include "RuntimeCompiler/SwapModuleEvent.h"

namespace SteelEngine {

    SE_CLASS()
    class Client : public Interface::INetwork
    {
        GENERATED_BODY
    private:
        SOCKET m_Socket;
        std::thread* m_Thread;
        std::queue<NetworkCommands::MessageData> m_Commands;
        NetworkCommands::INetworkCommand* m_Command;
        Variant* m_ServerInfo;

    public:
        Client();
        ~Client();

        bool m_SendingHugeBlock = false;

        SE_METHOD()
        Result Connect(const char* ip = "127.0.0.1");

        SE_METHOD()
        void Process();

        SE_METHOD()
        int Send(SOCKET sock, const char* buffer, Type::uint32 size) override;

        SE_METHOD()
        int Receive(SOCKET sock, char* buffer, Type::uint32 size) override;

        SE_METHOD()
        std::queue<NetworkCommands::MessageData>* GetCommands();

        void operator()(NetworkCommands::INetworkCommand* event);
    };

}