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

        Event::GlobalEvent::Add<Networking::SendMessageEvent>(this);
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

                m_PendingCommands[client] = std::queue<std::string>();

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

                char* buf = new char[bufferSize];

                ZeroMemory(buf, bufferSize);

                int bytesIn = Receive(sock, buf, bufferSize);

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
                    std::vector<std::string> splitted = split(buf, ' ');

                    if(splitted[0] == "get")
                    {
                        if(!m_PendingCommands[sock].empty())
                        {
                            std::string command = m_PendingCommands[sock].front();

                            Send(sock, command.c_str(), bufferSize);

                            m_PendingCommands[sock].pop();
                        }
                        else
                        {
                            std::string none = "NONE";

                            Send(sock, none.c_str(), bufferSize);
                        }
                    }
                    else if(splitted[0] == "createSwapModule")
                    {
                        size_t size;
                        std::string fileBuf;
                        std::string fileName = splitted[1];

                        fileBuf.resize(bufferSize);

                        sscanf(splitted[2].c_str(), "%zu", &size);

                        std::ofstream file(fileName, std::ios::binary);

                        for(size_t i = 0; i < size; i += bufferSize)
                        {
                            int bytesIn2 = recv(sock, &fileBuf[0], bufferSize, 0);

                            file.write(&fileBuf[0], bytesIn2);
                        }

                        file.close();

                        Send(sock, "DONE", bufferSize);

                        {
                            std::ifstream input(fileName.c_str(), std::ios::binary);
                            std::stringstream ss;
                            input.seekg(0, input.end);
                            size_t length = input.tellg();
                            input.seekg(0, input.beg);

                            ss << "createSwapModule " << fileName.c_str() << " " << length;

                            for(SocketMap::iterator it = m_PendingCommands.begin(); it != m_PendingCommands.end(); ++it)
                            {
                                if(sock != it->first)
                                {
                                    it->second.push(ss.str());
                                }
                            }

                            std::string buf;

                            buf.resize(bufferSize);

                            for(size_t i = 0; i < length; i += bufferSize)
                            {
                                input.seekg(i);
                                input.read(&buf[0], bufferSize);

                                for(SocketMap::iterator it = m_PendingCommands.begin(); it != m_PendingCommands.end(); ++it)
                                {
                                    if(sock != it->first)
                                    {
                                        it->second.push(buf);
                                    }
                                }
                            }
                        }

                        Event::GlobalEvent::Broadcast(SwapModuleEvent{ fileName });
                    }
                    else if(splitted[0] == "say")
                    {
                        for(SocketMap::iterator it = m_PendingCommands.begin(); it != m_PendingCommands.end(); ++it)
                        {
                            if(it->first != sock)
                            {
                                it->second.push(buf);
                            }
                        }

                        Send(sock, "DONE", bufferSize);
                    }
                    else if(splitted[0] == "createFile")
                    {
                        std::string fileName = splitted[1];
                        std::string fileBuf;

                        std::ofstream file(fileName, std::ios::binary);

                        while(1)
                        {
                            int bytesIn2 = recv(sock, &fileBuf[0], bufferSize, 0);

                            if(fileBuf != "DONE")
                            {
                                printf("%s\n", fileBuf.c_str());
                            }
                            else
                            {
                                break;
                            }
                        }

                        // for(size_t i = 0; i < size; i += bufferSize)
                        // {
                        //     int bytesIn2 = recv(sock, &fileBuf[0], bufferSize, 0);

                        //     file.write(&fileBuf[0], bytesIn2);
                        // }

                        file.close();

                        Send(sock, "DONE", bufferSize);
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

    void Server::operator()(const RecompiledEvent& event)
    {
        m_ServerInfo = Reflection::GetType("Server")->GetMetaData(SE_SERVER_INFO)->Convert<ServerInfo>();
    }

    void Server::operator()(const Networking::SendMessageEvent& event)
    {
        for(SocketMap::iterator it = m_PendingCommands.begin(); it != m_PendingCommands.end(); ++it)
        {
            it->second.push(event.m_Command);
        }
    }

}