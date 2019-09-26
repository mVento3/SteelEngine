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

namespace SteelEngine { namespace Editor { namespace ImGUI {

    void NetworkManagerWindow::HostServer()
    {
        Event::GlobalEvent::Broadcast(Network::HostServerEvent{});
    }

    void NetworkManagerWindow::ConnectToServer()
    {
        Event::GlobalEvent::Broadcast(Network::ConnectToServerEvent(m_ServerIP));

        m_Event = (Network::INetworkCommand*)Reflection::CreateInstance("SteelEngine::Network::SynchronizeProjectNCE");

        Event::LocalEvent<Network::ShouldOverrideEvent>* test =
            Reflection::GetType(m_Event)->Invoke("GetShouldOverrideEvent", m_Event).Convert<Event::LocalEvent<Network::ShouldOverrideEvent>*>();

        test->Add_(this);

        Event::GlobalEvent::Broadcast_(m_Event);

        // while(!m_Event->m_Deserialized || m_Event->m_Flow == Network::CommunicationFlow::CLIENT_TO_SERVER)
        // {

        // }
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

        // Reflection::GetType("Server")->Invoke("", net);
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

        if(m_ShouldOverrideFilesPopup)
        {
            ImGui::OpenPopup("##shouldOverrideFiles");

            m_ShouldOverrideFilesPopup = false;
        }

        if(ImGui::BeginPopup("##shouldOverrideFiles"))
        {
            ImGui::Text("Do you want override %s?", m_ShouldOverrideFilesEvent->m_File.c_str());

            if(ImGui::Button("Yes"))
            {
                m_ShouldOverrideFilesEvent->m_ShouldOverride = true;
                m_ShouldOverrideFilesEvent->m_IsSet = true;

                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();

            if(ImGui::Button("No"))
            {
                m_ShouldOverrideFilesEvent->m_ShouldOverride = false;
                m_ShouldOverrideFilesEvent->m_IsSet = true;

                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();

            if(ImGui::Button("Yes for all files"))
            {
                m_ShouldOverrideFilesEvent->m_ShouldOverrideForAll = true;
                m_ShouldOverrideFilesEvent->m_IsSet = true;

                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    void NetworkManagerWindow::operator()(const LoadedProjectEvent& event)
    {
        m_ProjectLoaded = true;
    }

    void NetworkManagerWindow::operator()(const RecompiledEvent& event)
    {

    }

    void NetworkManagerWindow::operator()(Network::ShouldOverrideEvent* event)
    {
        m_ShouldOverrideFilesEvent = event;
        m_ShouldOverrideFilesPopup = true;
    }

}}}