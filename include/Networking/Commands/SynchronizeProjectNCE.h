#pragma once

#include "Networking/INetworkCommand.h"

#include "Networking/Commands/SynchronizeProjectNCE.Generated.h"

#include "PythonCore/Scriptable.h"

#include "Networking/Events/ShouldOverrideEvent.h"

namespace SteelEngine { namespace Network {

    SE_CLASS(
        SteelEngine::ReflectionAttribute::NO_SERIALIZE,
        SteelEngine::ReflectionAttribute::NETWORK_COMMAND,
        SteelEngine::ReflectionAttribute::GENERATE_CAST_FUNCTIONS
    )
    class SynchronizeProjectNCE : public INetworkCommand, public Script::Python::Scriptable
    {
        GENERATED_BODY
    public:
        enum State
        {
            UP_TO_DATE,
            OVERRIDE,
            NONE
        };

    private:
        Event::LocalEvent<ShouldOverrideEvent> m_ShouldOverrideEvent;
        char* m_Buffer;
        size_t m_BufferSize;

    public:
        SynchronizeProjectNCE();
        ~SynchronizeProjectNCE();

        void ServerSide(Network::INetwork* network, SOCKET sock) override;
        void ClientSide(Network::INetwork* network, SOCKET sock) override;

        SE_METHOD()
        inline Event::LocalEvent<ShouldOverrideEvent>* GetShouldOverrideEvent() { return &m_ShouldOverrideEvent; }
    };

}}