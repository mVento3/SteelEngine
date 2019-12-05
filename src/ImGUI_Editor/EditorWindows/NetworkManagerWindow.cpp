#include "ImGUI_Editor/EditorWindows/NetworkManagerWindow.h"

#include "Networking/INetwork.h"
#include "Networking/GetNetworkInfo.h"

#include "Event/GlobalEvent.h"

#include "RuntimeReflection/Reflection.h"

#include "Networking/Events/HostServerEvent.h"
#include "Networking/Events/ConnectToServerEvent.h"

#include "Networking/NetworkManager.h"

#include "Utils/Json.h"

#include "VirtualProject/GetLoadedProjectPathEvent.h"

#include "Core/Core.h"

#include "imgui/imgui.h"

namespace SteelEngine { namespace Editor { namespace ImGUI {

    void NetworkManagerWindow::HostServer()
    {
        Event::GlobalEvent::Broadcast(Network::HostServerEvent{});
    }

    void NetworkManagerWindow::ConnectToServer()
    {
        Event::GlobalEvent::Broadcast(Network::ConnectToServerEvent(m_ServerIP));

        Network::INetworkManager* networkManager =
            *Reflection::GetType("SteelEngine::Core")->GetMetaData(Core::GlobalSystems::NETWORK_MANAGER)->Convert<Network::INetworkManager**>();

        std::vector<Network::INetworkCommand*> commands = networkManager->GetCommands();

        for(Type::uint32 i = 0; i < commands.size(); i++)
        {
            if(commands[i]->m_TypeID == Reflection::GetType("SteelEngine::Network::SynchronizeProjectNCE")->GetTypeID())
            {
                Event::GlobalEvent::Broadcast_(commands[i]);
            }
        }
    }

    NetworkManagerWindow::NetworkManagerWindow()
    {

    }

    NetworkManagerWindow::~NetworkManagerWindow()
    {

    }

    void NetworkManagerWindow::Init()
    {
        GetNetworkInfo info;

        Event::GlobalEvent::Broadcast_(&info);

        const Network::INetwork* net = info.m_Network;

        for(Type::uint32 i = 0; i < sizeof(m_ServerIP) / sizeof(char); i++)
        {
            m_ServerIP[i] = 0;
        }

        for(Type::uint32 i = 0; i < sizeof(m_Nickname) / sizeof(char); i++)
        {
            m_Nickname[i] = 0;
        }

        Event::GlobalEvent::Add<LoadedProjectEvent>(this);
    }

    void NetworkManagerWindow::Draw()
    {
        if(ImGui::BeginMainMenuBar())
        {
            if(ImGui::BeginMenu("Network"))
            {
                if(ImGui::MenuItem("Host server"))
                {
                    m_HostServerPopup = true;
                }

                if(ImGui::MenuItem("Connect"))
                {
                    m_ConnectToServerPopup = true;
                }

                ImGui::EndMenu();
            }

            if(ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                {
                    ImGui::Text("Some info like server status: connected to: ... or hosting");
                }
                ImGui::EndTooltip();
            }

            ImGui::EndMainMenuBar();
        }

        if(m_HostServerPopup)
        {
            ImGui::OpenPopup("##hostServerPopup");

            m_HostServerPopup = false;
        }

        if(m_ConnectToServerPopup)
        {
            ImGui::OpenPopup("##connectToTheServerPopup");

            m_ConnectToServerPopup = false;
        }

        if(ImGui::BeginPopup("##hostServerPopup"))
        {
            if(m_ProjectLoaded)
            {
                ImGui::Text("Do you want host server?");

                if(ImGui::Button("Host!"))
                {
                    HostServer();

                    ImGui::CloseCurrentPopup();
                }
            }
            else
            {
                ImGui::Text("First you need to load project that you want to host!");

                if(ImGui::Button("Close"))
                {
                    ImGui::CloseCurrentPopup();
                }
            }

            ImGui::EndPopup();
        }

        if(ImGui::BeginPopup("##connectToTheServerPopup"))
        {
            ImGui::InputText("Server ip", m_ServerIP, sizeof(m_ServerIP) / sizeof(char));
            ImGui::InputText("Nickname", m_Nickname, sizeof(m_Nickname) / sizeof(char));

            if(ImGui::Button("Connect"))
            {
                ConnectToServer();

                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    void NetworkManagerWindow::operator()(const LoadedProjectEvent& event)
    {
        m_ProjectLoaded = true;
    }

    void NetworkManagerWindow::OnRecompile(HotReloader::IRuntimeObject* oldObject)
    {
        EditorComponents::ImGUI::UserInterface::OnRecompile(oldObject);
        ImGui::SetCurrentContext((ImGuiContext*)m_Context);
    }

}}}