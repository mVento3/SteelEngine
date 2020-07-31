#include "Networking/Commands/SwapModuleNetCommandEvent.h"

namespace SteelEngine { namespace Network {

    SwapModuleNetCommandEvent::SwapModuleNetCommandEvent()
    {
        m_Header = SE_GET_TYPE_NAME(SwapModuleNetCommandEvent);

        m_Buffer = new char[1024];
    }

    SwapModuleNetCommandEvent::SwapModuleNetCommandEvent(const char* moduleName) :
        m_ModuleName(moduleName)
    {
        m_Header = SE_GET_TYPE_NAME(SwapModuleNetCommandEvent);

        printf("Swapping module over network: %s\n", moduleName);

        m_Buffer = new char[1024];
    }

    void SwapModuleNetCommandEvent::ServerSide(Network::INetwork* network, SOCKET sock)
    {
        Type::uint32 bufSize = Reflection::GetType("SteelEngine::Server")->GetMetaData(SERVER_INFO)->Convert<Type::uint32>();

        if(m_Flow == CommunicationFlow::SERVER_TO_CLIENT)
        {
            std::string fileBuf;

            fileBuf.resize(bufSize);

            std::ifstream input(((getBinaryLocation() / "Runtime/Swap/").string() + m_ModuleName + ".dll").c_str(), std::ios::binary);

            input.seekg(0, input.end);

            size_t length = input.tellg();

            input.seekg(0, input.beg);

            for(Type::uint32 i = 0; i < length; i += bufSize)
            {
                input.read(&fileBuf[0], bufSize);

                network->Receive(sock, m_Buffer, bufSize);
                network->Send(sock, fileBuf.c_str(), bufSize);
            }

            input.close();
            network->Receive(sock, m_Buffer, bufSize);
            network->Send(sock, "DONE", bufSize);
            fileBuf.clear();
        }
        else
        {
            std::string fileBuf;

            fileBuf.resize(bufSize);

            std::ofstream output(((getBinaryLocation() / "Runtime/Swap/").string() + m_ModuleName + ".dll").c_str(), std::ios::binary);

            while(1)
            {
                network->Receive(sock, &fileBuf[0], bufSize);
                network->Send(sock, "DONE", bufSize);

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

    void SwapModuleNetCommandEvent::ClientSide(Network::INetwork* network, SOCKET sock)
    {
        Type::uint32 bufSize = Reflection::GetType("SteelEngine::Server")->GetMetaData(SERVER_INFO)->Convert<Type::uint32>();

        if(m_Flow == CommunicationFlow::SERVER_TO_CLIENT)
        {
            std::string fileBuf;

            fileBuf.resize(bufSize);

            std::ofstream output(((getBinaryLocation() / "Runtime/Swap/").string() + m_ModuleName + ".dll").c_str(), std::ios::binary);

            while(1)
            {
                network->Send(sock, "DONE", bufSize);
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

            std::ifstream input(((getBinaryLocation() / "Runtime/Swap/").string() + m_ModuleName + ".dll").c_str(), std::ios::binary);
            input.seekg(0, input.end);
            size_t length = input.tellg();
            input.seekg(0, input.beg);

            for(Type::uint32 i = 0; i < length; i += bufSize)
            {
                input.read(&fileBuf[0], bufSize);

                network->Send(sock, fileBuf.c_str(), bufSize);
                network->Receive(sock, m_Buffer, bufSize);
            }

            input.close();

            network->Send(sock, "DONE", bufSize);
            network->Receive(sock, m_Buffer, bufSize);

            fileBuf.clear();
        }
    }

}}