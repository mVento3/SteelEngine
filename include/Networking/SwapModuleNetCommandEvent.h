#pragma once

#include "Networking/INetworkCommand.h"
#include "Networking/Config.h"

#include "string"
#include "fstream"
#include "queue"

#include "RuntimeReflection/ReflectionAttributes.h"

#include "SwapModuleNetCommandEvent.Generated.h"

namespace SteelEngine { namespace NetworkCommands {

    SE_CLASS(SteelEngine::ReflectionAttribute::SE_NO_SERIALIZE = true, SteelEngine::ReflectionAttribute::SE_NETWORK_COMMAND)
    struct SwapModuleNetCommandEvent : public INetworkCommand
    {
        GENERATED_BODY

        SE_VALUE(SteelEngine::ReflectionAttribute::SE_NET_VALUE)
        std::string m_ModuleName = "";

        SwapModuleNetCommandEvent()
        {
            strcpy(m_Header, SE_GET_TYPE_NAME(SwapModuleNetCommandEvent));
        }

        SwapModuleNetCommandEvent(const std::string& moduleName) :
            m_ModuleName(moduleName)
        {
            strcpy(m_Header, SE_GET_TYPE_NAME(SwapModuleNetCommandEvent));
        }

        SE_METHOD()
        void ServerSide(Interface::INetwork* network, SOCKET sock) override
        {
            Type::uint32 bufSize = Reflection::GetType("Server")->GetMetaData(SE_SERVER_INFO)->Convert<Type::uint32>();

            if(m_Flow == CommunicationFlow::SERVER_TO_CLIENT)
            {
                std::string fileBuf;

                fileBuf.resize(bufSize);

                std::ifstream input(m_ModuleName.c_str(), std::ios::binary);
                input.seekg(0, input.end);
                size_t length = input.tellg();
                input.seekg(0, input.beg);

                for(Type::uint32 i = 0; i < length; i += bufSize)
                {
                    input.read(&fileBuf[0], bufSize);

                    network->Send(sock, fileBuf.c_str(), bufSize);
                }

                input.close();
                network->Send(sock, "DONE", bufSize);
                fileBuf.clear();
            }
            else
            {
                std::string fileBuf;

                fileBuf.resize(bufSize);

                std::ofstream output(m_ModuleName.c_str(), std::ios::binary);

                while(1)
                {
                    network->Receive(sock, &fileBuf[0], bufSize);

                    if(strcmp(fileBuf.c_str(), "DONE") == 0)
                    {
                        break;
                    }
                    else
                    {
                        output.write(fileBuf.c_str(), bufSize);
                    }
                }

                output.close();
                Event::GlobalEvent::Broadcast(SteelEngine::SwapModuleEvent{ m_ModuleName });
                fileBuf.clear();
            }
        }

        SE_METHOD()
        void ClientSide(Interface::INetwork* network, SOCKET sock) override
        {
            Type::uint32 bufSize = Reflection::GetType("Server")->GetMetaData(SE_SERVER_INFO)->Convert<Type::uint32>();

            if(m_Flow == CommunicationFlow::SERVER_TO_CLIENT)
            {
                std::string fileBuf;

                fileBuf.resize(bufSize);

                std::ofstream output(m_ModuleName.c_str(), std::ios::binary);

                while(1)
                {
                    network->Receive(sock, &fileBuf[0], bufSize);

                    if(strcmp(fileBuf.c_str(), "DONE") == 0)
                    {
                        break;
                    }
                    else
                    {
                        output.write(fileBuf.c_str(), bufSize);
                    }
                }

                output.close();
                Event::GlobalEvent::Broadcast(SteelEngine::SwapModuleEvent{ m_ModuleName });
                fileBuf.clear();
            }
            else
            {
                std::string fileBuf;

                fileBuf.resize(bufSize);

                std::ifstream input(m_ModuleName.c_str(), std::ios::binary);
                input.seekg(0, input.end);
                size_t length = input.tellg();
                input.seekg(0, input.beg);

                for(Type::uint32 i = 0; i < length; i += bufSize)
                {
                    input.read(&fileBuf[0], bufSize);

                    network->Send(sock, fileBuf.c_str(), bufSize);
                }

                input.close();
                network->Send(sock, "DONE", bufSize);
                network->Send(sock, "GetNetCommandEvent", bufSize);
                fileBuf.clear();
            }
        }
    };

}}