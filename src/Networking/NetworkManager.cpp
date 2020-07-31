#include "Networking/NetworkManager.h"

#include "RuntimeReflection/Reflection.h"

#include "Core/Core.h"

#include "HotReloader/ReloadableInheritanceTrackKeeper.h"

#include "EditorComponents/ImGUI/UserInterface.h"

namespace SteelEngine { namespace Network {

    NetworkManager::NetworkManager()
    {
        m_Connected = new bool(false);
        m_Network = 0;
    }

    NetworkManager::~NetworkManager()
    {

    }

    void NetworkManager::Init()
    {
        IReflectionData const* const* types = Reflection::GetTypes();

        for(Type::uint32 i = 0; i < Reflection::GetTypesSize(); i++)
        {
            const IReflectionData* type = types[i];

            if(type->GetMetaData(Reflection::ReflectionAttribute::NETWORK_COMMAND)->Convert<bool>())
            {
                void** comm = type->Create_();

                HotReloader::InheritanceTrackKeeper* swapper = new HotReloader::ReloadableIneritanceTrackKeeper(type, comm);

                swapper->Get<Network::INetworkCommand>()->m_Commands = &m_CommandTypes;
                swapper->Get<Network::INetworkCommand>()->m_Swapper = swapper;

                IReflectionData::InheritancesVector& inhs = type->GetInheritances();

                for(Type::uint32 i = 0; i < inhs.Size(); i++)
                {
                    if(inhs[i]->GetTypeID() == typeid(EditorComponents::ImGUI::UserInterface).hash_code())
                    {
                        swapper->Get<EditorComponents::ImGUI::UserInterface>()->Init();

                        break;
                    }
                }

                m_CommandTypes.push_back(swapper);
            }
        }

        Event::GlobalEvent::Add<HostServerEvent>(this);
        Event::GlobalEvent::Add<ConnectToServerEvent>(this);
    }

    Type::uint32 NetworkManager::GetConnectedClientsCount()
    {
        return m_ConnectedClients.size();
    }

    void NetworkManager::operator()(GetNetworkInfo* event)
    {
        event->m_Network = m_Network;
    }

    void NetworkManager::operator()(const Network::ServerDisconnectedEvent& event)
    {
        *m_Connected = false;
    }

    void NetworkManager::operator()(const Network::ClientConnectedEvent& event)
    {
        m_ConnectedClients.push_back(event.m_ClientInfo);
    }

    void NetworkManager::operator()(const Network::ClientDisconnectedEvent& event)
    {
        for(ClientInfoVector::iterator it = m_ConnectedClients.begin(); it != m_ConnectedClients.end(); ++it)
        {
            if(it->m_Socket == event.m_Socket)
            {
                it->m_ConnectedToServer = false;

                break;
            }
        }
    }

    void NetworkManager::operator()(const HostServerEvent& event)
    {
        m_Network = (Network::INetwork*)Reflection::CreateInstance("SteelEngine::Server");

        m_Network->SetNetworkManager(this);

        Reflection::GetType("SteelEngine::Server")->Invoke("Start", m_Network);

        SE_INFO("Hosting server started!");

        m_Network->m_ClientConnectedEvent.Add(this);
        m_Network->m_ClientDisconnectedEvent.Add(this);

        Event::GlobalEvent::Add_<GetNetworkInfo>(this);
    }

    void NetworkManager::operator()(const ConnectToServerEvent& event)
    {
        m_Network = (Network::INetwork*)Reflection::CreateInstance("SteelEngine::Client");

        m_Network->SetNetworkManager(this);

        if(Reflection::GetType("SteelEngine::Client")->Invoke("Connect", m_Network, event.m_ServerIP.c_str()).Convert<Result>() == SE_TRUE)
        {
            SE_INFO("Connected to server successful!");

            *m_Connected = true;

            Reflection::GetType("SteelEngine::Client")->Invoke("Subprocess", m_Network);
        }

        m_Network->m_DisconnectEvent.Add(this);

        Event::GlobalEvent::Add_<GetNetworkInfo>(this);
    }

}}