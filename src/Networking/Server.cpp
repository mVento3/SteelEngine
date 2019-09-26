#include "Networking/Server.h"

#include "Utils/Utils.h"
#include "Utils/Network.h"

#include "Networking/INetworkManager.h"
#include "Networking/INetwork.h"

namespace SteelEngine {

    void Server::ProcessClient(Network::ClientThread* info)
    {
        Type::uint32 bufferSize = m_ServerInfo->Convert<ServerInfo>().m_BufferSize;
        int bytesIn = Receive(info->m_Socket, info->m_Buffer, bufferSize);
        size_t size = 0;
        size_t size2 = 0;
        std::string buf;

        Utils::receiveWholeDataS(
            this,
            buf,
            info->m_Buffer,
            info->m_Socket,
            bufferSize
        );

        if(bytesIn < 0)
        {
            printf("Client disconnected!\n");
#ifdef SE_WINDOWS
            closesocket(info->m_Socket);
#else
            closes(info->m_Socket);
#endif
            m_ClientDisconnectedEvent.BroadcastVaradic(info->m_Socket);
            info->m_Signal.set_value();
        }
        else if(bytesIn == 0)
        {
            printf("Client disconnected!\n");
#ifdef SE_WINDOWS
            closesocket(info->m_Socket);
#else
            closes(info->m_Socket);
#endif
            m_ClientDisconnectedEvent.BroadcastVaradic(info->m_Socket);
            info->m_Signal.set_value();
        }
        else
        {
            if(buf != "get")
            {
                size_t* strSizePtr = (size_t*)&buf[0];
                size_t strSize = *strSizePtr;

                strSizePtr++;

                char* bytes = (char*)strSizePtr;
                std::string data;

                for(Type::uint32 i = 0; i < strSize; i++)
                {
                    data.push_back(*bytes);
                    bytes++;
                }

                for(Type::uint32 i = 0; i < m_NetworkManager->GetCommands().size(); i++)
                {
                    Network::INetworkCommand* command = m_NetworkManager->GetCommands()[i];

                    if(strcmp(command->m_Header.c_str(), data.c_str()) == 0)
                    {
                        size_t size = 0;
                        size_t size2 = 0;
                        char* data = 0;
                        char* dataOrg = 0;

                        command->CalculateSize(size);
                        command->Deserialize(&buf[0], size);

                        Send(info->m_Socket, "DONE", 1024);

                        command->ServerSide(this, info->m_Socket);
                        command->CalculateSize(size);

                        data = new char[size];
                        dataOrg = data;
                        size2 = size;

                        command->m_Flow = Network::CommunicationFlow::SERVER_TO_CLIENT;
                        command->Serialize(data, size);

                        while(1)
                        {
                            Receive(info->m_Socket, info->m_Buffer, 1024);
                            Send(info->m_Socket, data, 1024);

                            if(size2 > 0)
                            {
                                size_t size3 = size2;

                                for(Type::uint32 i = 0; i < 1024 && i < size3; i++)
                                {
                                    data++;
                                    size2--;
                                }
                            }
                            else
                            {
                                Receive(info->m_Socket, info->m_Buffer, 1024);
                                Send(info->m_Socket, "END", 1024);

                                break;
                            }
                        }

                        delete[] dataOrg;

                        break;
                    }
                }
            }
            else
            {
                std::queue<Network::INetworkCommand*>* commands = &m_Commands[info->m_Socket];

                if(!commands->empty())
                {
                    Network::INetworkCommand* command = commands->front();

                    size_t size = 0;
                    char* data = 0;

                    command->CalculateSize(size);

                    data = new char[size];

                    command->m_Flow = Network::CommunicationFlow::SERVER_TO_CLIENT;
                    command->Serialize(data, size);

                    Send(info->m_Socket, data, 1024);
                    Receive(info->m_Socket, info->m_Buffer, 1024);

                    command->ServerSide(this, info->m_Socket);

                    Receive(info->m_Socket, info->m_Buffer, 1024);
                    Send(info->m_Socket, "DONE", 1024);

                    command->CalculateSize(size);
                    command->Deserialize(info->m_Buffer, size);

                    delete[] data;

                    commands->pop();
                }
                else
                {
                    Send(info->m_Socket, "none", 1024);
                }
            }
        }
    }

    Server::Server()
    {
        m_ServerInfo = Reflection::GetType("SteelEngine::Server")->GetMetaData(SERVER_INFO);
    }

    Server::~Server()
    {

    }
    
    void Server::Start()
    {
        WSADATA wsData;
        WORD ver = MAKEWORD(2, 2);

        if(WSAStartup(ver, &wsData) != 0)
        {
            printf("Failed to init winsock!\n");

            return;
        }

        m_ListeningSocket = socket(AF_INET, SOCK_STREAM, 0);

        if(m_ListeningSocket == INVALID_SOCKET)
        {
            printf("Error on listening!\n");

            return;
        }

        sockaddr_in hint;

        hint.sin_family = AF_INET;
        hint.sin_port = htons(m_ServerInfo->Convert<ServerInfo>().m_Port);

        inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

        bind(m_ListeningSocket, (sockaddr*)&hint, sizeof(hint));
        listen(m_ListeningSocket, SOMAXCONN);

        std::vector<IReflectionData*> types = Reflection::GetTypes();

        for(Type::uint32 i = 0; i < types.size(); i++)
        {
            IReflectionData* type = types[i];

            if(type->GetMetaData(ReflectionAttribute::NETWORK_COMMAND)->Convert<bool>())
            {
                Network::INetworkCommand* comm = (Network::INetworkCommand*)type->Create();

                comm->m_Commands = &m_NetworkManager->GetCommands();
            }
        }

        Event::GlobalEvent::Add_<Network::INetworkCommand>(this);

        m_WaitForConnectionThread = new std::thread([this]()
        {
            while(1)
            {
                m_Object->Update();

                Sleep(100);
            }
        });

        m_Disconnected.Add(this);
    }

    void Server::Update()
    {
        sockaddr_in clientAddress;
        int clientAddressSize = sizeof(clientAddress);
        SOCKET client = accept(
            m_ListeningSocket,
            (sockaddr*)&clientAddress,
            &clientAddressSize
        );

        printf(
            "New connection: %s:%i\n",
            inet_ntoa(clientAddress.sin_addr),
            ntohs(clientAddress.sin_port)
        );

        m_Commands[client] = std::queue<Network::INetworkCommand*>();

        Network::ClientInfo clientInfo = Network::ClientInfo(clientAddress, this, client);

        clientInfo.m_ConnectedToServer = true;

        m_ClientConnectedEvent.Broadcast(Network::ClientConnectedEvent{ clientInfo });

        Network::ClientThread* thread = new Network::ClientThread(client);

        thread->m_Thread = std::thread([this, &thread](std::future<void> object)
        {
            Type::uint32 bufferSize = m_ServerInfo->Convert<ServerInfo>().m_BufferSize;

            thread->m_Buffer = new char[bufferSize];

            while(object.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout)
            {
                ProcessClient(thread);
            }

            m_Disconnected.Broadcast(Network::ClientDisconnectedEvent(thread->m_Socket));
        }, std::move(thread->m_FutureObject));

        m_ClientsThreads.push_back(thread);
    }

    int Server::Send(SOCKET sock, const char* buffer, Type::uint32 size)
    {
        return send(sock, buffer, size, 0);
    }

    int Server::Receive(SOCKET sock, char* buffer, Type::uint32 size)
    {
        return recv(sock, buffer, size, 0);
    }

    std::queue<Network::INetworkCommand*>* Server::GetCommands(SOCKET sock)
    {
        return &m_Commands[sock];
    }

    void Server::operator()(const RecompiledEvent& event)
    {
        // m_ServerInfo = Reflection::GetType("Server")->GetMetaData(SERVER_INFO)->Convert<ServerInfo>();
    }

    void Server::operator()(Network::INetworkCommand* event)
    {
        event->m_Flow = Network::CommunicationFlow::SERVER_TO_CLIENT;

        for(SocketMap::iterator it = m_Commands.begin(); it != m_Commands.end(); ++it)
        {
            it->second.push(event);
        }
    }

    void Server::operator()(const Network::ClientDisconnectedEvent& event)
    {
        for(Type::uint32 i = 0; i < m_ClientsThreads.size(); i++)
        {
            Network::ClientThread* thread = m_ClientsThreads[i];

            if(thread->m_Socket == event.m_Socket)
            {
                m_ClientsThreads.erase(m_ClientsThreads.begin() + i);

                break;
            }
        }
    }

}