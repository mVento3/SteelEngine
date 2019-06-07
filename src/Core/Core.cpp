#include "Core/Core.h"

#include "windows.h"
#include "iostream"
#include "string"

#include "FileSystem/FileSystem.h"

#include "Serialization/Serialization.h"

#include "SDL_events.h"

namespace SteelEngine {

    void Core::Loop()
    {
        Core* core = (Core*)m_Object;

        while(m_Running)
        {
            m_Object->Update();
        }

        ((Core*)m_Object)->m_Window->Close();

        Cleanup();
    }

    void Core::Cleanup()
    {

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
        //meta = Reflection::GetType("Core")->GetProperty("ta")->GetMetaData(SteelEngine::Core::Lol::TES);
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

    // Graphics stuff
        m_Window = (Interface::IWindow*)Reflection::CreateInstance("VulkanWindow");
        m_Renderer = (Interface::IRenderer*)Reflection::CreateInstance("VulkanRenderer", m_Window);

        m_Window->SetTitle("Test Window!");
        m_Window->SetWidth(800);
        m_Window->SetHeight(600);

        std::function<void(void*)> func = [](void* event_)
        {
            SDL_Event* event = (SDL_Event*)event_;

            if(event->type == SDL_QUIT)
            {
                
            }
        };

        Reflection::GetType("VulkanWindow")->Invoke("SetProcessEventsCallback", m_Window, func);

        if(m_Window->Create() == SE_FALSE)
        {
            return SE_FALSE;
        }

        Event::GlobalEvent::Add<Interface::IWindow::WindowCloseEvent>((Core*)m_Object);

        if(m_Renderer->Init() == SE_FALSE)
        {
            return SE_FALSE;
        }
    // ---------------------------------------------------------------------

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
        m_RuntimeCompiler->m_Object->Update();
        m_Logger->m_Object->Update();
        m_Renderer->m_Object->Update();
        m_Window->m_Object->Update();

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

        // SE_INFO("CZESC %i", 33);
        // SE_FATAL("LOL");

        // if(Reflection::GetType("Core")->GetMetaData(EngineInfo::IS_SERVER)->Convert<bool>())
        // {
        //     NetworkCommands::INetworkCommand* ev = new NetworkCommands::SwapModuleEvent("bin/Runtime/Swap/s83k.1.dll\0");

        //     Event::GlobalEvent::Broadcast_(ev);

        //     delete ev;
        // }

        Sleep(100);
    }

    struct Test2
    {
        const char* a = "aaaa";
        int aa = 10;
    };

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

        Serialization ser;

        //int a[] = { 2, 2 };
        std::vector<int> a = { 2, 2 };

        std::vector<const char*> aa =
        {
            "fff\0",
            "aaa\0"
        };

        ser.Serialize(10, 2.3231f, a, "aa", aa);

        int _int;
        float _float;
        std::vector<int> _intVector;
        std::vector<char*> _strVector;
        char** _strArray;
        char* _strArray2;
        std::string str;

        _strArray = &_strArray2;

        // char* res = Serialization::SerializeStream(10, 2.3231f, a, "aa", aa);

        // Serialization::DeserializeStream(res, _int, _float, _intVector, str, _strVector);

        ser.Deserialize(_int, _float, _intVector, _strArray, _strVector);

        // Serialization ser2;

        // ser2.SerializeData(Test2{ "aeerer", 222 });

        // Test2 t;

        // ser2.DeserializeData(t);

        Loop();
    }

    void Core::Stop()
    {
        if(!m_Running)
        {
            return;
        }

        m_Running = false;
    }

    void Core::SetPathVariant(EnginePathVariant variant)
    {
        m_EnginePathVariant = variant;
    }

    void Core::operator()(const RecompiledEvent& event)
    {
        Event::GlobalEvent::Remove<Interface::IWindow::WindowCloseEvent>(event.m_Object);
        Event::GlobalEvent::Add<Interface::IWindow::WindowCloseEvent>((Core*)m_Object);
    }

    void Core::operator()(const Interface::IWindow::WindowCloseEvent& event)
    {
        Stop();
    }

}