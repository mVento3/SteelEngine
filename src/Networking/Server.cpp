#include "Networking/Server.h"

#include "Utils/Utils.h"
#include "Utils/Network.h"

#include "Networking/INetworkManager.h"
#include "Networking/INetwork.h"

namespace SteelEngine {

    void Server::ProcessClient()
    {
        Type::uint32 bufferSize = m_ServerInfo->Convert<ServerInfo>().m_BufferSize;
        int bytesIn = Receive(m_SD, m_Buffer, bufferSize);
        size_t size = 0;
        size_t size2 = 0;
        std::string buf;

        Utils::receiveWholeDataS(
            this,
            buf,
            m_Buffer,
            m_SD,
            bufferSize
        );

        if(bytesIn < 0)
        {
            printf("Client disconnected!\n");
#ifdef SE_WINDOWS
            closesocket(m_SD);
#else
            closes(info->m_Socket);
#endif
            m_ClientDisconnectedEvent.BroadcastVariadic(m_SD);
        }
        else if(bytesIn == 0)
        {
            printf("Client disconnected!\n");
#ifdef SE_WINDOWS
            closesocket(m_SD);
#else
            closes(info->m_Socket);
#endif
            m_ClientDisconnectedEvent.BroadcastVariadic(m_SD);
        }
        else
        {
            if(buf != "get")
            {
                printf("Getting command!\n");

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
                    // Network::INetworkCommand* command = m_NetworkManager->GetCommands()[i];
                    HotReloader::InheritanceTrackKeeper* swapper = m_NetworkManager->GetCommands()[i];
                    Network::INetworkCommand* command = swapper->Get<Network::INetworkCommand>();

                    if(strcmp(command->m_Header.c_str(), data.c_str()) == 0)
                    {
                        size_t size = 0;
                        size_t size2 = 0;
                        char* data = 0;
                        char* dataOrg = 0;

                        command->CalculateSize(size);
                        command->Deserialize(&buf[0], size);

                        Send(m_SD, "DONE", 1024);

                        command->ServerSide(this, m_SD);
                        command->CalculateSize(size);

                        data = new char[size];
                        dataOrg = data;
                        size2 = size;

                        command->m_Flow = Network::CommunicationFlow::SERVER_TO_CLIENT;
                        command->Serialize(data, size);

                        while(1)
                        {
                            Receive(m_SD, m_Buffer, 1024);
                            Send(m_SD, data, 1024);

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
                                Receive(m_SD, m_Buffer, 1024);
                                Send(m_SD, "END", 1024);

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
                std::queue<Network::INetworkCommand*>* commands = &m_Commands[m_SD];

                if(!commands->empty())
                {
                    printf("Sending command!\n");

                    Network::INetworkCommand* command = commands->front();

                    size_t size = 0;
                    char* data = 0;

                    command->CalculateSize(size);

                    data = new char[size];

                    command->m_Flow = Network::CommunicationFlow::SERVER_TO_CLIENT;
                    command->Serialize(data, size);

                    Send(m_SD, data, 1024);
                    Receive(m_SD, m_Buffer, 1024);

                    command->ServerSide(this, m_SD);

                    Receive(m_SD, m_Buffer, 1024);
                    Send(m_SD, "DONE", 1024);

                    command->CalculateSize(size);
                    command->Deserialize(m_Buffer, size);

                    delete[] data;

                    commands->pop();
                }
                else
                {
                    Send(m_SD, "none", 1024);
                }
            }
        }
    }

    Server::Server()
    {
        m_ServerInfo = Reflection::GetType("SteelEngine::Server")->GetMetaData(SERVER_INFO);
        memset(m_Buffer, 0, 1024);

        for(uint32_t i = 0; i < MAX_CLIENTS; i++)
        {
            m_Clients[i] = 0;
        }
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

        hint.sin_family = AF_INET;
        hint.sin_port = htons(m_ServerInfo->Convert<ServerInfo>().m_Port);

        inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

        hintLen = sizeof(hint);

        bind(m_ListeningSocket, (sockaddr*)&hint, hintLen);

        IReflectionData const* const* types = Reflection::GetTypes();

        for(Type::uint32 i = 0; i < Reflection::GetTypesSize(); i++)
        {
            const IReflectionData* type = types[i];

            if(type->GetMetaData(Reflection::ReflectionAttribute::NETWORK_COMMAND)->Convert<bool>())
            {
                Network::INetworkCommand* comm = (Network::INetworkCommand*)type->Create();

                comm->m_Commands = &m_NetworkManager->GetCommands();
            }
        }

        listen(m_ListeningSocket, 3);

        m_WaitForConnectionThread = new std::thread([this]()
        {
            while(1)
            {
                Update();

                Sleep(100);
            }
        });

        Event::GlobalEvent::Add_<Network::INetworkCommand>(this);

        m_Disconnected.Add(this);
    }

    void Server::Update()
    {
        FD_ZERO(&m_ReadFDS);

        FD_SET(m_ListeningSocket, &m_ReadFDS);

        m_MaxSD = m_ListeningSocket;

        for(uint32_t i = 0; i < MAX_CLIENTS; i++)
        {
            m_SD = m_Clients[i];

            if(m_SD > 0)
            {
                FD_SET(m_SD, &m_ReadFDS);
            }

            if(m_SD > m_MaxSD)
            {
                m_MaxSD = m_SD;
            }
        }

        m_Activity = select(m_MaxSD + 1, &m_ReadFDS, NULL, NULL, NULL);

        if(m_Activity < 0)
        {
            printf("Select error!\n");
        }

        if(FD_ISSET(m_ListeningSocket, &m_ReadFDS))
        {
            if((m_NewConnection = accept(m_ListeningSocket, (sockaddr*)&hint, &hintLen)) < 0)
            {
                printf("Accept error!\n");
            }

            for(uint32_t i = 0; i < MAX_CLIENTS; i++)
            {
                if(m_Clients[i] == 0)
                {
                    m_Clients[i] = m_NewConnection;

                    break;
                }
            }

            m_Commands[m_NewConnection] = std::queue<Network::INetworkCommand*>();

            Network::ClientInfo clientInfo = Network::ClientInfo(hint, this, m_NewConnection);

            clientInfo.m_ConnectedToServer = true;

            m_ClientConnectedEvent.Broadcast(Network::ClientConnectedEvent{ clientInfo });
        }

        for(uint32_t i = 0; i < MAX_CLIENTS; i++)
        {
            m_SD = m_Clients[i];

            if(FD_ISSET(m_SD, &m_ReadFDS))
            {
                ProcessClient();
            }
        }
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
        // for(Type::uint32 i = 0; i < m_ClientsThreads.size(); i++)
        // {
        //     Network::ClientThread* thread = m_ClientsThreads[i];

        //     if(thread->m_Socket == event.m_Socket)
        //     {
        //         m_ClientsThreads.erase(m_ClientsThreads.begin() + i);

        //         break;
        //     }
        // }
    }

}