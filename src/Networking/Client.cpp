#include "Networking/Client.h"

#include "Utils/Utils.h"
#include "Utils/Time.h"

#include "fstream"

#include "FileSystem/FileSystem.h"

namespace SteelEngine {

    Client::Client()
    {
        m_ServerInfo = Reflection::GetType("Server")->GetMetaData(SE_SERVER_INFO);
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
        hint.sin_port = htons(m_ServerInfo->Convert<ServerInfo>().m_Port);
        
        inet_pton(AF_INET, ip, &hint.sin_addr);

        int connectionResult = connect(m_Socket, (sockaddr*)&hint, sizeof(hint));

        if(connectionResult == SOCKET_ERROR)
        {
            printf("Failed to connect to the server!\n");
            closesocket(m_Socket);
            WSACleanup();

            return SE_FALSE;
        }

        m_Command = (NetworkCommands::INetworkCommand*)Reflection::CreateInstance("GetNetCommandEvent");

        Event::GlobalEvent::Add_<NetworkCommands::INetworkCommand>(this);

        return SE_TRUE;
    }

    void Client::Process()
    {
        m_Thread = new std::thread([this]()
        {
            Type::uint32 bufferSize = m_ServerInfo->Convert<ServerInfo>().m_BufferSize;

            while(1)
            {
                m_Command->ClientSide(this, m_Socket);

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

    std::queue<NetworkCommands::MessageData>* Client::GetCommands()
    {
        return &m_Commands;
    }

    void Client::operator()(const RecompiledEvent& event)
    {
        
    }

    void Client::operator()(NetworkCommands::INetworkCommand* event)
    {
        NetworkCommands::MessageData data;

        data.m_Size = Reflection::GetType(event->m_Header)->GetMetaData("sizeof")->Convert<size_t>();
        data.m_Data = new char[data.m_Size];

        event->m_Flow = NetworkCommands::CommunicationFlow::CLIENT_TO_SERVER;
        
        event->Serialize(data.m_Data);

        m_Commands.push(data);
    }

}