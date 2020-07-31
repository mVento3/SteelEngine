#pragma once

#include "EditorComponents/ImGUI/UserInterface.h"

#include "Editor/ImGUI/ReflectionAttribs.h"
#include "Editor/ImGUI/WindowType.h"

#include "VirtualProject/LoadedProjectEvent.h"

#include "RuntimeReflection/Macro.h"

#include "Networking/INetwork.h"
#include "Networking/INetworkCommand.h"

#include "PythonCore/Scriptable.h"

#include "Editor/ImGUI/EditorWindows/NetworkManagerWindow.Generated.h"

namespace SteelEngine { namespace Editor { namespace ImGUI {

    SE_CLASS(
        Editor::ReflectionAttributes::WINDOW_TYPE = SteelEngine::Editor::WindowType::STATIC,
        Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS,
        Editor::ReflectionAttributes::EDITOR_WINDOW,
        Reflection::ReflectionAttribute::HOT_RELOAD
    )
    class NetworkManagerWindow : public EditorComponents::ImGUI::UserInterface, public Script::Python::Scriptable, public HotReloader::IRuntimeObject
    {
        GENERATED_BODY
    private:
        bool m_HostServerPopup = false;
        bool m_ConnectToServerPopup = false;

        char m_ServerIP[32];
        char m_Nickname[32];

        bool m_ProjectLoaded = false;

        void HostServer();
        void ConnectToServer();

    public:
        NetworkManagerWindow();
        ~NetworkManagerWindow();

        void Init() override;
        void Draw() override;

        void DrawConnectToServer();

        void OnRecompile(HotReloader::IRuntimeObject* oldObject) override;

        void operator()(const LoadedProjectEvent& event);
    };

}}}