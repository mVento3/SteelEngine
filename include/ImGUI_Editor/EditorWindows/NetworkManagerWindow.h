#pragma once

#include "EditorComponents/ImGUI/UserInterface.h"

#include "ImGUI_Editor/ReflectionAttribs.h"
#include "ImGUI_Editor/SceneType.h"

#include "VirtualProject/LoadedProjectEvent.h"

#include "RuntimeReflection/Macro.h"

#include "Networking/INetwork.h"
#include "Networking/INetworkCommand.h"

#include "PythonCore/Scriptable.h"

#include "ImGUI_Editor/EditorWindows/NetworkManagerWindow.Generated.h"

namespace SteelEngine { namespace Editor { namespace ImGUI {

    SE_CLASS(
        SteelEngine::Editor::ReflectionAttributes::SCENE_TYPE = SteelEngine::Editor::SceneType::EDITOR_SCENE | SteelEngine::Editor::SceneType::START_MENU_SCENE,
        SteelEngine::ReflectionAttribute::GENERATE_CAST_FUNCTIONS,
        SteelEngine::Editor::ReflectionAttributes::EDITOR_WINDOW
    )
    class NetworkManagerWindow : public EditorComponents::ImGUI::UserInterface, public Script::Python::Scriptable
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