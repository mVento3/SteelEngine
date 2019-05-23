#pragma once

#include "string"
#include "fstream"
#include "queue"

#include "Event/globalevent.h"

#include "Networking/Config.h"
#include "Networking/INetworkCommand.h"
#include "Networking/SwapModuleNetCommandEvent.h"

#include "NetworkCommands.Generated.h"

#define BUFSIZE 1024

namespace SteelEngine { namespace NetworkCommands {

    SE_CLASS(SteelEngine::ReflectionAttribute::SE_NO_SERIALIZE, SteelEngine::ReflectionAttribute::SE_NETWORK_COMMAND)
    struct GetNetCommandEvent : public INetworkCommand
    {
        char m_HeaderComp[HEADER_SIZE];
        char* m_Buffer;
        Type::uint32 m_BufferSize;

        GetNetCommandEvent()
        {
            strcpy(m_Header, SE_GET_TYPE_NAME(GetNetCommandEvent));

            m_BufferSize = Reflection::GetType("Server")->GetMetaData(SE_SERVER_INFO)->Convert<Type::uint32>();

            m_Buffer = new char[m_BufferSize];
        }

        SE_METHOD()
        char* Serialize(char* data) override
        {
            return INetworkCommand::Serialize(data);
        }

        SE_METHOD()
        char* Deserialize(char* data) override
        {
            return INetworkCommand::Deserialize(data);
        }

        SE_METHOD()
        void ServerSide(Interface::INetwork* network, SOCKET sock) override
        {
            Variant commands = Reflection::GetType("Server")->Invoke("GetCommands", network, sock);
            std::queue<MessageData>* queue = commands.Convert<std::queue<NetworkCommands::MessageData>*>();

            if(!queue->empty())
            {
                MessageData data = queue->front();

                network->Send(sock, data.m_Data, data.m_Size);

                for(int i = 0; i < HEADER_SIZE; i++)
                {
                    m_HeaderComp[i] = data.m_Data[i];
                }

                if(strcmp(m_HeaderComp, "SwapModuleNetCommandEvent") == 0)
                {
                    NetworkCommands::SwapModuleNetCommandEvent s;

                    s.Deserialize(data.m_Data);
                    s.ServerSide(network, sock);
                }

                queue->pop();
            }
            else
            {
                network->Send(sock, "NoneNetCommandEvent", m_BufferSize);
            }

            commands.Clear();
        }

        SE_METHOD()
        void ClientSide(Interface::INetwork* network, SOCKET sock) override
        {
            Variant commands = Reflection::GetType("Client")->Invoke("GetCommands", network);
            std::queue<MessageData>* queue = commands.Convert<std::queue<NetworkCommands::MessageData>*>();

            if(!queue->empty())
            {
                while(!queue->empty())
                {
                    MessageData data = queue->front();

                    network->Send(sock, data.m_Data, m_BufferSize);

                    for(int i = 0; i < HEADER_SIZE; i++)
                    {
                        m_HeaderComp[i] = data.m_Data[i];
                    }

                    if(strcmp(m_HeaderComp, "SwapModuleNetCommandEvent") == 0)
                    {
                        NetworkCommands::SwapModuleNetCommandEvent s;

                        s.Deserialize(data.m_Data);
                        s.ClientSide(network, sock);
                    }

                    queue->pop();
                }
            }
            else
            {
                network->Send(sock, "GetNetCommandEvent", m_BufferSize);
            }

            commands.Clear();
            network->Receive(sock, m_Buffer, m_BufferSize);

            for(int i = 0; i < HEADER_SIZE; i++)
            {
                m_HeaderComp[i] = m_Buffer[i];
            }

            if(strcmp(m_HeaderComp, "NoneNetCommandEvent") == 0)
            {

            }
            else if(strcmp(m_HeaderComp, "SwapModuleNetCommandEvent") == 0)
            {
                NetworkCommands::SwapModuleNetCommandEvent s;

                s.Deserialize(m_Buffer);
                s.ClientSide(network, sock);
            }
        }
    };

}}