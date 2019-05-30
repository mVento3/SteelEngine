#pragma once

#include "Networking/INetwork.h"

#include "RuntimeCompiler/SwapModuleEvent.h"
#include "RuntimeCompiler/IRuntimeObject.h"

#include "RuntimeReflection/Reflection.h"
#include "RuntimeReflection/ReflectionAttributes.h"
#include "RuntimeReflection/Macro.h"

#include "Utils/Utils.h"

#include "Serialization/Serialization.h"

#include "vector"

#define HEADER_SIZE 50

namespace SteelEngine { namespace NetworkCommands {

    struct MessageData
    {
        char* m_Data;
        size_t m_Size;
    };

    enum CommunicationFlow
    {
        SERVER_TO_CLIENT,
        CLIENT_TO_SERVER
    };

    SE_CLASS(SteelEngine::ReflectionAttribute::SE_NO_SERIALIZE)
    struct INetworkCommand : public Interface::IRuntimeObject
    {
        CommunicationFlow m_Flow;
        char m_Header[HEADER_SIZE];
        std::vector<INetworkCommand*>* m_Commands;

        virtual char* Serialize(char *data)
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

}}