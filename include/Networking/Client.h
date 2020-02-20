#pragma once

#include "Networking/INetwork.h"
#include "Networking/Config.h"
#include "Networking/INetworkCommand.h"

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/Reflection.h"

#include "Core/Result.h"

#include "ws2tcpip.h"
#include "thread"
#include "queue"
#include "string"

#include "Event/globalevent.h"

#include "HotReloader/Events/SwapModuleEvent.h"

#include "Networking/Client.Generated.h"

namespace SteelEngine {

    SE_CLASS(
        Reflection::ReflectionAttribute::HOT_RELOAD
    )
    class Client : public Network::INetwork
    {
        GENERATED_BODY
    private:
        SOCKET m_Socket;
        std::thread* m_Thread;
        std::queue<Network::INetworkCommand*> m_Commands;
        char* m_Buffer;

        Network::INetworkManager* m_NetworkManager;

    public:
        Client();
        ~Client();

        SE_METHOD()
        Result Connect(const char* ip = "127.0.0.1");

        SE_METHOD()
        void Process();

        SE_METHOD()
        int Send(SOCKET sock, const char* buffer, Type::uint32 size) override;

        SE_METHOD()
        int Receive(SOCKET sock, char* buffer, Type::uint32 size) override;

        int SendSerialized(const std::string& buffer, Type::uint32 size);

        void SetNetworkManager(Network::INetworkManager* networkManager) override { m_NetworkManager = networkManager; }

        SE_METHOD()
        std::queue<Network::INetworkCommand*>* GetCommands()
        {
            return &m_Commands;
        }

        void operator()(Network::INetworkCommand* event);
    };

}