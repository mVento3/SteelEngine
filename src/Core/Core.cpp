#include "Core/Core.h"

#include "windows.h"
#include "iostream"
#include "string"

#include "FileSystem/FileSystem.h"

namespace SteelEngine {

    void Core::Loop()
    {
        while(m_Running)
        {
            m_Object->Update();
        }
    }

    Result Core::Init()
    {
        printf("Bin location: %s\n", FileSystem::GetBinaryLocation().string().c_str());

        m_Logger = (Interface::ILogger*)Reflection::CreateInstance("Logger", "log.log");

        if(m_Logger->Init() == SE_FALSE)
        {
            return SE_FALSE;
        }

        ((RuntimeDatabase*)ModuleManager::GetModule("RuntimeDatabase"))->m_GlobalLogger = m_Logger;

        meta2 = Reflection::GetType("Core")->GetProperty("ta")->GetMetaData(SteelEngine::ReflectionAttribute::SE_RUNTIME_SERIALIZE);
        meta = Reflection::GetType("Core")->GetProperty("ta")->GetMetaData(SteelEngine::Core::Lol::TES);
        meta3 = Reflection::GetType("Core")->GetProperty("ta")->GetMetaData("lol");

        m_RuntimeCompiler =
            (SteelEngine::RuntimeCompiler*)SteelEngine::Reflection::CreateInstance("RuntimeCompiler");

        if(m_RuntimeCompiler->Initalize() == SteelEngine::SE_FALSE)
        {
            SE_FATAL("Runtime Compiler initialization failed!");

            return SE_FALSE;
        }

        if(Reflection::GetType("Core")->GetMetaData(EngineInfo::IS_SERVER)->Convert<bool>())
        {
            m_Network = (Interface::INetwork*)Reflection::CreateInstance("Server");

            Reflection::GetType("Server")->Invoke("Start", m_Network);

            std::thread* serverThread = new std::thread([this]()
            {
                while(1)
                {
                    m_Network->m_Object->Update();

                    Sleep(1);
                }
            });
        }
        else
        {
            m_Network = (Interface::INetwork*)Reflection::CreateInstance("Client");

            std::string ip = Reflection::GetType("Core")->GetMetaData(EngineInfo::SERVER_IP)->Convert<std::string>();

            if(Reflection::GetType("Client")->Invoke("Connect", m_Network, ip).Convert<Result>() == SE_TRUE)
            {
                SE_INFO("Connected successful!");

                Reflection::GetType("Client")->Invoke("Process", m_Network);
            }
        }

        return SE_TRUE;
    }

    Core::Core()
    {
        m_Running = false;
    }

    Core::~Core()
    {
        
    }

    void Core::Update()
    {
        m_RuntimeCompiler->Update();
        m_Logger->Update();

        // if(meta2->IsValid())
        // {
        //     printf("b %i\n", meta2->Convert<int>());
        // }

        // if(meta->IsValid())
        // {
        //     printf("a %i\n", meta->Convert<int>());
        // }

        // if(meta3->IsValid())
        // {
        //     printf("c %i\n", meta3->Convert<int>());
        // }

        //SE_INFO("CZESC %i", 33);
        //SE_FATAL("LOL");

        // if(Reflection::GetType("Core")->GetMetaData(EngineInfo::IS_SERVER)->Convert<bool>())
        // {
        //     NetworkCommands::INetworkCommand* ev = new NetworkCommands::SwapModuleEvent("bin/Runtime/Swap/s83k.1.dll\0");

        //     Event::GlobalEvent::Broadcast_(ev);

        //     delete ev;
        // }

        Sleep(100);
    }

    void Core::Start()
    {
        if(m_Running)
        {
            return;
        }
        
        m_Running = true;

        if(Init() == SE_FALSE)
        {
            return;
        }

        Loop();
    }

    void Core::Stop()
    {
        if(!m_Running)
        {
            return;
        }
    }

    void Core::SetPathVariant(EnginePathVariant variant)
    {
        m_EnginePathVariant = variant;
    }

    void Core::operator()(const RecompiledEvent& event)
    {
        // meta = Reflection::GetType("Core")->GetProperty("ta")->GetMetaData(SteelEngine::Core::Lol::TES);
    }

}