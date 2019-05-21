#pragma once

#include "winsock2.h"

#include "RuntimeCompiler/IRuntimeObject.h"

#include "Networking/SendMessageEvent.h"

namespace SteelEngine { namespace Interface {

    struct INetwork : public IRuntimeObject
    {
        virtual int Receive(SOCKET sock, char* buffer, Type::uint32 size) { return -1; }
        virtual int Send(SOCKET sock, const char* buffer, Type::uint32 size) { return -1; }

        virtual void operator()(const Networking::SendMessageEvent& event) { }
    };

}}