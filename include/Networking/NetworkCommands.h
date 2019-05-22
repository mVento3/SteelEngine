#pragma once

#include "string"
#include "fstream"

#include "Networking/INetwork.h"

#include "Event/globalevent.h"

#include "RuntimeCompiler/SwapModuleEvent.h"

#define BUFSIZE 1024
#define HEADER_SIZE 50

namespace SteelEngine { namespace NetworkCommands {

    enum CommunicationFlow
    {
        SERVER_TO_CLIENT,
        CLIENT_TO_SERVER
    };

    struct INetworkCommand
    {
        size_t m_Size;
        CommunicationFlow m_Flow;
        char m_Header[HEADER_SIZE];

        virtual char* Serialize(char *data) const
        {
            char* h = (char*)data;

            for(int i = 0; i < HEADER_SIZE; i++)
            {
                *h = m_Header[i];
                h++;
            }

            CommunicationFlow* cm = (CommunicationFlow*)h;

            *cm = m_Flow;

            cm++;

            return (char*)cm;
        }

        virtual char* Deserialize(char *data)
        {
            char* h = (char*)data;

            for(int i = 0; i < HEADER_SIZE; i++)
            {
                h++;
            }

            CommunicationFlow* cm = (CommunicationFlow*)h;

            m_Flow = *cm;

            cm++;

            return (char*)cm;
        }

        virtual void ServerSide(Interface::INetwork* network, SOCKET sock) { }
        virtual void ClientSide(Interface::INetwork* network, SOCKET sock) { }
    };

    struct SendMessageEvent
    {
        std::string m_Command;
        size_t m_Size = 0;
    };

    struct NoneEvent : public INetworkCommand
    {
        NoneEvent()
        {
            m_Size = sizeof(NoneEvent);

            strcpy(m_Header, "NoneEvent\0");
        }

        char* Serialize(char* data) const override
        {
            return INetworkCommand::Serialize(data);
        }

        char* Deserialize(char* data) override
        {
            return INetworkCommand::Deserialize(data);
        }

        void ServerSide(Interface::INetwork* network, SOCKET sock) override
        {

        }

        void ClientSide(Interface::INetwork* network, SOCKET sock) override
        {

        }
    };

    struct GetEvent : public INetworkCommand
    {
        GetEvent()
        {
            m_Size = sizeof(GetEvent);

            strcpy(m_Header, "GetEvent\0");
        }

        char* Serialize(char* data) const override
        {
            return INetworkCommand::Serialize(data);
        }

        char* Deserialize(char* data) override
        {
            return INetworkCommand::Deserialize(data);
        }

        void ServerSide(Interface::INetwork* network, SOCKET sock) override
        {

        }

        void ClientSide(Interface::INetwork* network, SOCKET sock) override
        {

        }
    };

    struct SwapModuleEvent : public INetworkCommand
    {
        std::string m_ModuleName;

        SwapModuleEvent()
        {

        }

        SwapModuleEvent(const std::string& moduleName) :
            m_ModuleName(moduleName)
        {
            m_Size = sizeof(SwapModuleEvent);

            strcpy(m_Header, "SwapModuleEvent\0");
        }

        char* Serialize(char* data) const override
        {
            char* p = INetworkCommand::Serialize(data);

            for(int i = 0; i < strlen(m_ModuleName.c_str()) + 1; i++)
            {
                *p = m_ModuleName[i];
                p++;
            }

            return p;
        }

        char* Deserialize(char* data) override
        {
            char* p = INetworkCommand::Deserialize(data);

            while(1)
            {
                m_ModuleName.push_back(*p);
                p++;

                if(*p == '\0')
                {
                    p++;
                    break;
                }
            }

            return p;
        }

        void ServerSide(Interface::INetwork* network, SOCKET sock) override
        {
            if(m_Flow == CommunicationFlow::SERVER_TO_CLIENT)
            {
                std::string fileBuf;

                fileBuf.resize(1024);

                std::ifstream input(m_ModuleName.c_str(), std::ios::binary);
                input.seekg(0, input.end);
                size_t length = input.tellg();
                input.seekg(0, input.beg);

                for(Type::uint32 i = 0; i < length; i += 1024)
                {
                    input.read(&fileBuf[0], 1024);

                    network->Send(sock, fileBuf.c_str(), 1024);
                }

                network->Send(sock, "DONE", 1024);
            }
            else
            {
                std::string fileBuf;

                fileBuf.resize(1024);

                std::ofstream output(m_ModuleName.c_str(), std::ios::binary);

                while(1)
                {
                    network->Receive(sock, &fileBuf[0], 1024);

                    if(strcmp(fileBuf.c_str(), "DONE") == 0)
                    {
                        break;
                    }
                    else
                    {
                        output.write(fileBuf.c_str(), 1024);
                    }
                }

                output.close();
                Event::GlobalEvent::Broadcast(SteelEngine::SwapModuleEvent{ m_ModuleName });
            }
        }

        void ClientSide(Interface::INetwork* network, SOCKET sock) override
        {
            if(m_Flow == CommunicationFlow::SERVER_TO_CLIENT)
            {
                std::string fileBuf;

                fileBuf.resize(1024);

                std::ofstream output(m_ModuleName.c_str(), std::ios::binary);

                while(1)
                {
                    network->Receive(sock, &fileBuf[0], 1024);

                    if(strcmp(fileBuf.c_str(), "DONE") == 0)
                    {
                        break;
                    }
                    else
                    {
                        output.write(fileBuf.c_str(), 1024);
                    }
                }

                output.close();
                Event::GlobalEvent::Broadcast(SteelEngine::SwapModuleEvent{ m_ModuleName });
            }
            else
            {
                std::string fileBuf;

                fileBuf.resize(1024);

                std::ifstream input(m_ModuleName.c_str(), std::ios::binary);
                input.seekg(0, input.end);
                size_t length = input.tellg();
                input.seekg(0, input.beg);

                for(Type::uint32 i = 0; i < length; i += 1024)
                {
                    input.read(&fileBuf[0], 1024);

                    network->Send(sock, fileBuf.c_str(), 1024);
                }

                network->Send(sock, "DONE", 1024);
                input.close();

                network->Send(sock, "GetEvent", 1024);
            }
        }
    };

}}