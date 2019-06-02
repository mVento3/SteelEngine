#pragma once

#include "string"
#include "fstream"
#include "queue"

#include "Event/globalevent.h"

#include "Networking/Config.h"
#include "Networking/INetworkCommand.h"
#include "Networking/SwapModuleNetCommandEvent.h"

#include "GetNetCommandEvent.Generated.h"

#define BUFSIZE 1024

namespace SteelEngine { namespace NetworkCommands {

    SE_CLASS(SteelEngine::ReflectionAttribute::SE_NO_SERIALIZE, SteelEngine::ReflectionAttribute::SE_NETWORK_COMMAND)
    struct GetNetCommandEvent : public INetworkCommand
    {
        GENERATED_BODY

        std::string m_HeaderComp;
        char* m_Buffer;
        Type::uint32 m_BufferSize;

        GetNetCommandEvent()
        {
            m_Header = SE_GET_TYPE_NAME(GetNetCommandEvent);

            m_BufferSize = Reflection::GetType("Server")->GetMetaData(SE_SERVER_INFO)->Convert<Type::uint32>();

            m_Buffer = new char[m_BufferSize];
        }

        SE_METHOD()
        void ServerSide(Interface::INetwork* network, SOCKET sock) override
        {
            Variant commands = Reflection::GetType("Server")->Invoke("GetCommands", network, sock);
            std::queue<char*>* queue = commands.Convert<std::queue<char*>*>();

            if(!queue->empty())
            {
                char* data = queue->front();

                network->Send(sock, data, m_BufferSize);

                Serialization::DeserializeStream(data, m_HeaderComp);

                for(Type::uint32 i = 0; i < m_Commands->size(); i++)
                {
                    NetworkCommands::INetworkCommand* command = m_Commands->at(i);

                    if(command->m_Header == m_HeaderComp)
                    {
                        size_t totalSize = 0;

                        command->Deserialize(data, totalSize);
                        command->ServerSide(network, sock);

                        break;
                    }
                }

                queue->pop();
            }
            else
            {
                char* cmd = Serialization::SerializeStream("NoneNetCommandEvent", m_Flow);

                network->Send(sock, cmd, m_BufferSize);

                delete[] cmd;
            }

            commands.Clear();
        }

        SE_METHOD()
        void ClientSide(Interface::INetwork* network, SOCKET sock) override
        {
            Variant commands = Reflection::GetType("Client")->Invoke("GetCommands", network);
            std::queue<char*>* queue = commands.Convert<std::queue<char*>*>();

            if(!queue->empty())
            {
                while(!queue->empty())
                {
                    char* data = queue->front();

                    network->Send(sock, data, m_BufferSize);

                    Serialization::DeserializeStream(data, m_HeaderComp);

                    for(Type::uint32 i = 0; i < m_Commands->size(); i++)
                    {
                        NetworkCommands::INetworkCommand* command = m_Commands->at(i);

                        if(strcmp(command->m_Header.c_str(), m_HeaderComp.c_str()) == 0)
                        {
                            size_t totalSize = 0;

                            command->Deserialize(data, totalSize);
                            command->ClientSide(network, sock);

                            break;
                        }
                    }

                    queue->pop();
                }
            }
            else
            {
                char* cmd = Serialization::SerializeStream("GetNetCommandEvent", m_Flow);

                network->Send(sock, cmd, m_BufferSize);

                delete[] cmd;
            }

            commands.Clear();
            network->Receive(sock, m_Buffer, m_BufferSize);

            Serialization::DeserializeStream(m_Buffer, m_HeaderComp);

            for(Type::uint32 i = 0; i < m_Commands->size(); i++)
            {
                NetworkCommands::INetworkCommand* command = m_Commands->at(i);

                if(strcmp(command->m_Header.c_str(), m_HeaderComp.c_str()) == 0)
                {
                    size_t totalSize = 0;

                    command->Deserialize(m_Buffer, totalSize);
                    command->ClientSide(network, sock);

                    break;
                }
            }
        }
    };

}}