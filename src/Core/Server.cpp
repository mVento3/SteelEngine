#include "Core/Server.h"

#include "Utils/Utils.h"

#include "RuntimeCompiler/SwapModuleEvent.h"

namespace SteelEngine {

    Server::Server()
    {
        m_ServerInfo = Reflection::GetType("Server")->GetMetaData(SE_SERVER_INFO)->Convert<ServerInfo>();
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
        hint.sin_port = htons(m_ServerInfo.m_Port);
        hint.sin_addr.S_un.S_addr = INADDR_ANY;

        bind(m_ListeningSocket, (sockaddr*)&hint, sizeof(hint));
        listen(m_ListeningSocket, SOMAXCONN);

        FD_ZERO(&m_Master);
        FD_SET(m_ListeningSocket, &m_Master);

        std::vector<IReflectionData*> types = Reflection::GetTypes();

        for(Type::uint32 i = 0; i < types.size(); i++)
        {
            IReflectionData* type = types[i];

            if(type->GetMetaData(ReflectionAttribute::SE_NETWORK_COMMAND)->Convert<bool>())
            {
                m_CommandTypes.push_back((NetworkCommands::INetworkCommand*)type->Create());
            }
        }

        Type::uint32 bufferSize = m_ServerInfo.m_BufferSize;

        m_Buffer = new char[bufferSize];

        Event::GlobalEvent::Add_<NetworkCommands::INetworkCommand>(this);
    }

    void Server::Update()
    {
        fd_set copy = m_Master;

        int socketCount = select(0, &copy, nullptr, nullptr, nullptr);

        for(int i = 0; i < socketCount; i++)
        {
            SOCKET sock = copy.fd_array[i];

            if(sock == m_ListeningSocket)
            {
                sockaddr_in clientAddress;
                int clientAddressSize = sizeof(clientAddress);
                SOCKET client = accept(
                    m_ListeningSocket,
                    (sockaddr*)&clientAddress,
                    &clientAddressSize
                );

                FD_SET(client, &m_Master);

                printf(
                    "New connection: %s:%i\n",
                    inet_ntoa(clientAddress.sin_addr),
                    ntohs(clientAddress.sin_port)
                );
            }
            else
            {
                Type::uint32 bufferSize = m_ServerInfo.m_BufferSize;
                int bytesIn = Receive(sock, m_Buffer, bufferSize);

                if(bytesIn < 0)
                {
                    printf("Client disconnected!\n");
                    #ifdef SE_WINDOWS
                    closesocket(sock);
                    #else
                    closes(sock);
                    #endif
                    FD_CLR(sock, &m_Master);
                }
                else if(bytesIn == 0)
                {
                    printf("Client disconnected!\n");
                    #ifdef SE_WINDOWS
                    closesocket(sock);
                    #else
                    closes(sock);
                    #endif
                    FD_CLR(sock, &m_Master);
                }
                else
                {
                    for(int i = 0; i < HEADER_SIZE; i++)
                    {
                        m_Header[i] = m_Buffer[i];
                    }

                    for(Type::uint32 i = 0; i < m_CommandTypes.size(); i++)
                    {
                        NetworkCommands::INetworkCommand* command = m_CommandTypes[i];

                        if(strcmp(command->m_Header, m_Header) == 0)
                        {
                            command->Deserialize(m_Buffer);
                            command->ServerSide(this, sock);
                        }
                    }
                }
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

    std::queue<NetworkCommands::MessageData>* Server::GetCommands(SOCKET sock)
    {
        return &m_Commands[sock];
    }

    void Server::operator()(const RecompiledEvent& event)
    {
        m_ServerInfo = Reflection::GetType("Server")->GetMetaData(SE_SERVER_INFO)->Convert<ServerInfo>();
    }

    void Server::operator()(NetworkCommands::INetworkCommand* event)
    {  
        NetworkCommands::MessageData data;

        data.m_Size = Reflection::GetType(event->m_Header)->GetMetaData("sizeof")->Convert<size_t>();
        data.m_Data = new char[data.m_Size];

        event->m_Flow = NetworkCommands::CommunicationFlow::SERVER_TO_CLIENT;

        event->Serialize(data.m_Data);

        for(SocketMap::iterator it = m_Commands.begin(); it != m_Commands.end(); ++it)
        {
            it->second.push(data);
        }
    }

}