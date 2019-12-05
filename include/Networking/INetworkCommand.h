#pragma once

#include "Networking/INetwork.h"

#include "HotReloader/Events/SwapModuleEvent.h"

#include "HotReloader/IRuntimeObject.h"

#include "RuntimeReflection/Reflection.h"
#include "RuntimeReflection/Macro.h"

#include "Utils/Utils.h"

#include "Serialization/Serialization.h"

#include "vector"

#include "Core/ReflectionAttributes.h"

#define HEADER_SIZE 50

namespace SteelEngine { namespace Network {

    enum CommunicationFlow
    {
        SERVER_TO_CLIENT,
        CLIENT_TO_SERVER
    };

    SE_CLASS(SteelEngine::ReflectionAttribute::NO_SERIALIZE)
    struct INetworkCommand : public HotReloader::IRuntimeObject
    {
        SE_VALUE(SteelEngine::ReflectionAttribute::NET_VALUE)
        CommunicationFlow m_Flow;

        SE_VALUE(SteelEngine::ReflectionAttribute::NET_VALUE)
        std::string m_Header;

        bool m_Deserialized = false;
        bool m_Busy = false;

        const std::vector<INetworkCommand*>* m_Commands;

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

            Serialization::DeserializeType(totalSize, m_Header, out, &out);
            Serialization::DeserializeType(totalSize, m_Flow, out, &out);

            return out;
        }

        virtual void CalculateSize(size_t& totalSize)
        {
            Serialization::CalculateTotalSize(totalSize, m_Header);
            Serialization::CalculateTotalSize(totalSize, m_Flow);
        }

        virtual void ServerSide(Network::INetwork* network, SOCKET sock) { }
        virtual void ClientSide(Network::INetwork* network, SOCKET sock) { }
    };

}}