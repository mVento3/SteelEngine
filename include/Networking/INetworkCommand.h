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
        char* m_Data = 0;
        size_t m_Size = 0;
    };

    enum CommunicationFlow
    {
        SERVER_TO_CLIENT,
        CLIENT_TO_SERVER
    };

    SE_CLASS(SteelEngine::ReflectionAttribute::SE_NO_SERIALIZE)
    struct INetworkCommand : public Interface::IRuntimeObject
    {
        SE_VALUE(SteelEngine::ReflectionAttribute::SE_NET_VALUE)
        CommunicationFlow m_Flow;

        SE_VALUE(SteelEngine::ReflectionAttribute::SE_NET_VALUE)
        std::string m_Header;

        std::vector<INetworkCommand*>* m_Commands;

        virtual char* Serialize(char* data, size_t& totalSize)
        {
            char* out = data;

            Serialization::SerializeType(totalSize, m_Header, out, &out);
            Serialization::SerializeType(totalSize, m_Flow, out, &out);

            return out;
        }

        virtual char* Deserialize(char* data, size_t& totalSize)
        {
            char* out = data;

            size_t* stringSizePtr = (size_t*)out;

            totalSize = *stringSizePtr;
            stringSizePtr++;
            totalSize -= sizeof(size_t);

            out = (char*)stringSizePtr;

            Serialization::DeserializeType(totalSize, m_Header, out, &out);
            Serialization::DeserializeType(totalSize, m_Flow, out, &out);

            return out;
        }

        virtual void CalculateSize(size_t& totalSize)
        {
            Serialization::CalculateTotalSize(totalSize, m_Header);
            Serialization::CalculateTotalSize(totalSize, m_Flow);
        }

        virtual void ServerSide(Interface::INetwork* network, SOCKET sock) { }
        virtual void ClientSide(Interface::INetwork* network, SOCKET sock) { }
    };

}}