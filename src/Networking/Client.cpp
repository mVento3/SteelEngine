#include "Networking/Client.h"

#include "Utils/Utils.h"
#include "Utils/Time.h"

#include "fstream"

#include "FileSystem/FileSystem.h"

#include "Networking/INetworkManager.h"

namespace SteelEngine {

    Client::Client()
    {

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
        hint.sin_port = htons(Reflection::GetType("SteelEngine::Server")->GetMetaData(SERVER_INFO)->Convert<ServerInfo>().m_Port);
        
        inet_pton(AF_INET, ip, &hint.sin_addr);

        if(connect(m_Socket, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR)
        {
            printf("Failed to connect to the server!\n");
            closesocket(m_Socket);
            WSACleanup();

            return SE_FALSE;
        }

        Event::GlobalEvent::Add_<Network::INetworkCommand>(this);

        m_BufferSize = Reflection::GetType("SteelEngine::Server")->GetMetaData(SteelEngine::SERVER_INFO)->Convert<ServerInfo>().m_BufferSize;
        m_Buffer = new char[m_BufferSize];

        return SE_TRUE;
    }

    void Client::Subprocess()
    {
        bool* status = Reflection::GetType("SteelEngine::Network::NetworkManager")->Invoke("GetConnectionStatus", m_NetworkManager).Convert<bool*>();

        m_Thread = new std::thread([this, status]()
        {
            while(*status)
            {
                if(!m_Commands.empty())
                {
                    Network::INetworkCommand* command = m_Commands.front();

                    size_t size = 0;
                    size_t size2 = 0;
                    std::string data;

                    command->CalculateSize(size);
                    size2 = size;

                    data.resize(size);

                    command->m_Flow = Network::CommunicationFlow::CLIENT_TO_SERVER;
                    command->Serialize(&data[0], size);

                    size2 += sizeof(size_t);

                    SendSerialized(data, m_BufferSize);
                    Receive(m_Socket, m_Buffer, m_BufferSize);

                    if(size2 > m_BufferSize)
                    {
                        size2 -= m_BufferSize;

                        char* tmp = &data[0];

                        while(1)
                        {
                            if(size2 > 0)
                            {
                                size_t size3 = size2;

                                for(Type::uint32 i = 0; i < m_BufferSize && i < size3; i++)
                                {
                                    tmp++;
                                    size2--;
                                }
                            }
                            else
                            {
                                Send(m_Socket, "DONE", m_BufferSize);
                                Receive(m_Socket, m_Buffer, m_BufferSize);

                                break;
                            }

                            Send(m_Socket, tmp, m_BufferSize);
                            Receive(m_Socket, m_Buffer, m_BufferSize);
                        }
                    }

                    command->ClientSide(this, m_Socket);

                    std::string data2;
                    std::string buf;

                    buf.resize(m_BufferSize);

                    while(1)
                    {
                        Send(m_Socket, "DONE", m_BufferSize);
                        Receive(m_Socket, &buf[0], m_BufferSize);

                        if(strcmp(&buf[0], "END") == 0)
                        {
                            break;
                        }
                        else
                        {
                            data2.append(buf);
                        }
                    }

                    command->CalculateSize(size);
                    command->Deserialize(&data2[0], size);
                    command->m_Deserialized = true;

                    m_Commands.pop();
                }
                else
                {
                    SendSerialized("get", m_BufferSize);
                    Receive(m_Socket, m_Buffer, m_BufferSize);

                    if(strcmp(m_Buffer, "none") != 0)
                    {
                        size_t* strSizePtr = (size_t*)m_Buffer;
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
                            HotReloader::InheritanceTrackKeeper* swapper = m_NetworkManager->GetCommands()[i];
                            Network::INetworkCommand* command = swapper->Get<Network::INetworkCommand>();

                            if(strcmp(command->m_Header.c_str(), data.c_str()) == 0)
                            {
                                size_t size = 0;
                                char* data = 0;

                                command->CalculateSize(size);
                                command->Deserialize(m_Buffer, size);
                                Send(m_Socket, "DONE", m_BufferSize);
                                command->ClientSide(this, m_Socket);
                                command->CalculateSize(size);

                                data = new char[size];

                                command->m_Flow = Network::CommunicationFlow::CLIENT_TO_SERVER;
                                command->Serialize(data, size);
                                Send(m_Socket, data, m_BufferSize);
                                Receive(m_Socket, m_Buffer, m_BufferSize);

                                delete[] data;

                                break;
                            }
                        }
                    }
                }

                Sleep(1);
            }

            printf("Server connection lost!\n");
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

    int Client::SendSerialized(const std::string& buffer, Type::uint32 size)
    {
        char* ser = Serialization::SerializeStream(buffer);

        return Send(m_Socket, ser, size);
    }

    void Client::operator()(Network::INetworkCommand* event)
    {
        event->m_Flow = Network::CommunicationFlow::CLIENT_TO_SERVER;

        m_Commands.push(event);
    }

}