#pragma once

#include "Networking/INetworkCommand.h"

#include "PythonCore/Scriptable.h"

#include "Networking/Events/ShouldOverrideEvent.h"

#include "EditorComponents/ImGUI/UserInterface.h"

#include "Editor/ImGUI/ReflectionAttribs.h"
#include "Editor/ImGUI/WindowType.h"

#include "Networking/Commands/SynchronizeProjectNCE.Generated.h"

namespace SteelEngine { namespace Network {

    SE_CLASS(
        SteelEngine::Reflection::ReflectionAttribute::NETWORK_COMMAND,
        SteelEngine::Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS,
        SteelEngine::Editor::ReflectionAttributes::WINDOW_TYPE = SteelEngine::Editor::WindowType::STATIC,
        // SteelEngine::EditorComponents::ImGUI::UserInterface::SEPARATE_WINDOW,
        SteelEngine::Reflection::ReflectionAttribute::RUNTIME_SERIALIZE,
        Reflection::ReflectionAttribute::HOT_RELOAD
    )
    class SynchronizeProjectNCE :
        // SE_INHERITANCE(SteelEngine::Reflection::ReflectionAttribute::DO_NOT_GENERATE_CAST_FUNCTIONS)
        public INetworkCommand,
        public Script::Python::Scriptable,
        public EditorComponents::ImGUI::UserInterface,
        public HotReloader::IRuntimeObject
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
        SE_VALUE(
            SteelEngine::Reflection::ReflectionAttribute::NO_SERIALIZE
        )
        Event::LocalEvent<ShouldOverrideEvent> m_ShouldOverrideEvent;
        ShouldOverrideEvent* m_ShouldOverrideEventData;
        bool m_DrawShouldOverridePopup;

        char* m_Buffer;
        size_t m_BufferSize;

    public:
        SynchronizeProjectNCE();
        ~SynchronizeProjectNCE();

        void ServerSide(Network::INetwork* network, SOCKET sock) override;
        void ClientSide(Network::INetwork* network, SOCKET sock) override;

        void Init() override;
        void Draw() override;

        void OnRecompile(HotReloader::IRuntimeObject* oldObject) override;

        void operator()(ShouldOverrideEvent* event);
    };

}}