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

        std::vector<IReflectionData*> types = Reflection::GetTypes();

        for(Type::uint32 i = 0; i < types.size(); i++)
        {
            IReflectionData* type = types[i];

            if(type->GetMetaData(ReflectionAttribute::SE_NETWORK_COMMAND)->Convert<bool>())
            {
                NetworkCommands::INetworkCommand* comm = (NetworkCommands::INetworkCommand*)type->Create();

                comm->m_Commands = &m_CommandTypes;

                m_CommandTypes.push_back(comm);
            }
        }

        m_Command = (NetworkCommands::INetworkCommand*)Reflection::CreateInstance("GetNetCommandEvent");

        m_Command->m_Commands = &m_CommandTypes;

        Event::GlobalEvent::Add_<NetworkCommands::INetworkCommand>(this);

        return SE_TRUE;
    }

    void Client::Process()
    {
        m_Thread = new std::thread([this]()
        {
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

        event->m_Flow = NetworkCommands::CommunicationFlow::CLIENT_TO_SERVER;

        event->CalculateSize(data.m_Size);

        data.m_Size += sizeof(size_t);

        data.m_Data = new char[data.m_Size];

        char* temp = data.m_Data;

        size_t* stringSizePtr = (size_t*)temp;

        *stringSizePtr = data.m_Size;
        stringSizePtr++;
        data.m_Size -= sizeof(size_t);

        temp = (char*)stringSizePtr;

        event->Serialize(temp, data.m_Size);

        m_Commands.push(data);
    }

}