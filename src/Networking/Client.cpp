#include "Networking/Client.h"

#include "Utils/Utils.h"

#include "fstream"

#include "FileSystem/FileSystem.h"

namespace SteelEngine {

    Client::Client()
    {
        m_ServerInfo = Reflection::GetType("Server")->GetMetaData(SE_SERVER_INFO)->Convert<ServerInfo>();
    }

    Client::~Client()
    {

    }

    Result Client::Connect(const char* ip)
    {
        WSADATA wsData;
        WORD ver = MAKEWORD(2, 2);

        if(WSAStartup(ver, &wsData) != 0)
        {
            printf("Failed to init winsock!\n");

            return SE_FALSE;
        }

        m_Socket = socket(AF_INET, SOCK_STREAM, 0);

        if(m_Socket == INVALID_SOCKET)
        {
            printf("Error on listening!\n");

            return SE_FALSE;
        }

        sockaddr_in hint;

        hint.sin_family = AF_INET;
        hint.sin_port = htons(m_ServerInfo.m_Port);
        
        inet_pton(AF_INET, ip, &hint.sin_addr);

        int connectionResult = connect(m_Socket, (sockaddr*)&hint, sizeof(hint));

        if(connectionResult == SOCKET_ERROR)
        {
            printf("Failed to connect to the server!\n");
            closesocket(m_Socket);
            WSACleanup();

            return SE_FALSE;
        }

        Event::GlobalEvent::Add<Networking::SendMessageEvent>(this);

        return SE_TRUE;
    }

    void Client::Process()
    {
        m_Thread = new std::thread([this]()
        {
            Type::uint32 bufferSize = m_ServerInfo.m_BufferSize;

            while(1)
            {
                std::string buf;

                buf.resize(bufferSize);

                if(!m_PendingCommands.empty())
                {
                    while(!m_PendingCommands.empty())
                    {
                        std::string command = m_PendingCommands.front();

                        Send(m_Socket, command.c_str(), bufferSize);

                        m_PendingCommands.pop();
                    }
                }
                else
                {
                    Send(m_Socket, "get", bufferSize);
                }

                Receive(m_Socket, (char*)&buf[0], bufferSize);

                std::vector<std::string> splitted = split(buf, ' ');

                if(splitted[0] == "DONE")
                {

                }
                else if(splitted[0] == "say")
                {
                    printf("%s\n", splitted[1].c_str());
                }
                else if(splitted[0] == "createSwapModule")
                {
                    size_t size;
                    std::string fileBuf;
                    std::string fileName = splitted[1];

                    filesystem::path p = fileName;

                    std::string fin = FileSystem::GetBinaryLocation().string() + "/Runtime/Swap/" + p.filename().string();

                    fileBuf.resize(bufferSize);

                    sscanf(splitted[2].c_str(), "%zu", &size);

                    std::ofstream file(fin, std::ios::binary);

                    for(size_t i = 0; i < size; i += bufferSize)
                    {
                        Send(m_Socket, "get", bufferSize);

                        int bytesIn2 = recv(m_Socket, &fileBuf[0], bufferSize, 0);

                        file.write(&fileBuf[0], bytesIn2);
                    }

                    file.close();

                    Send(m_Socket, "DONE", bufferSize);

                    Event::GlobalEvent::Broadcast(SwapModuleEvent{ fin });
                }

                Sleep(1);
            }
        });
    }

    int Client::Send(SOCKET sock, const char* buffer, Type::uint32 size)
    {
        return send(sock, buffer, size, 0);
    }

    int Client::Receive(SOCKET sock, char* buffer, Type::uint32 size)
    {
        return recv(sock, buffer, size, 0);
    }

    void Client::operator()(const RecompiledEvent& event)
    {
        m_ServerInfo = Reflection::GetType("Server")->GetMetaData(SE_SERVER_INFO)->Convert<ServerInfo>();
    }

    void Client::operator()(const Networking::SendMessageEvent& event)
    {
        m_PendingCommands.push(event.m_Command);
    }

}