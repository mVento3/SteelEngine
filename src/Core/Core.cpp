#include "Core/Core.h"

#include "windows.h"
#include "iostream"
#include "string"

#include "Serialization/Serialization.h"

#include "FileWatcher/FileWatcher.h"

#include "FileSystem/FileSystem.h"

#include "SDL_events.h"

#include "RuntimeReflection/ReflectionGenerator.h"

#include "Profiler/ScopeTimer.h"
#include "Profiler/Manager.h"

namespace SteelEngine {

    void Core::Loop()
    {
        Core** core = (Core**)&m_Object;

        while(m_Running)
        {
            m_Object->Update();
        }

        (*m_Renderer)->Cleanup();
        (*core)->m_Window->Close();

        Cleanup();
    }

    void Core::Cleanup()
    {

    }

    Result Core::Init()
    {
        SE_PROFILE_FUNC;
        // TODO: Delete this!!
        Profiler::Manager a;

        Reflection::GetType("SteelEngine::Core")->SetMetaData(GlobalSystems::PROFILER, &Reflection::CreateInstance("SteelEngine::Profiler::Manager")->m_Object);

        std::string logPath = FileSystem::Get("se_init_log").string();

        m_Logger = (ILogger*)Reflection::CreateInstance("SteelEngine::Logger", logPath.c_str());

        if(!m_Logger || m_Logger->Init() == SE_FALSE)
        {
            printf("Failed to setup logger!\n");

            return SE_FALSE;
        }

        Reflection::GetType("SteelEngine::Core")->SetMetaData(GlobalSystems::LOGGER, &m_Logger->m_Object);

        SE_INFO("Initializing core!");

        SE_WARNING("TEST!");
        SE_ERROR("TEST!");
        SE_FATAL("TEST!");

        std::ifstream configFile(getBinaryLocation() / "config.json");

        configFile >> m_CompileConfig;
        Reflection::GetType("SteelEngine::Core")->SetMetaData(ReflectionAttribute::PROJECTS_PATH, (std::string)m_CompileConfig["projects_path"]);

        m_CompileConfig = m_CompileConfig["compiler"];

        Event::GlobalEvent::Add_<GetCompileConfigEvent>(this);

        m_Python = (Script::IPython*)Reflection::CreateInstance("SteelEngine::Script::PythonCore");

        if(m_Python->Init() == SE_FALSE)
        {
            SE_FATAL("Failed to initialize python!");

            return SE_FALSE;
        }
        else
        {
            SE_INFO("Python initialization successful!");
        }

        Reflection::GetType("SteelEngine::Core")->SetMetaData(GlobalSystems::PYTHON, m_Python);
        Reflection::GetType("SteelEngine::Core")->SetMetaData(
            Graphics::IRenderer::SELECTED_RENDER_API,
            Graphics::IRenderer::VULKAN_API
        );

        std::vector<SteelEngine::IReflectionData*> types =
            Reflection::GetTypesByMetaData(ReflectionAttribute::REFLECTION_MODULE, [](Variant* key) -> bool
            {
                return key->Convert<bool>();
            });

        for(Type::uint32 i = 0; i < types.size(); i++)
        {
            m_ReflectionModules.push_back(types[i]->Create());
        }

        Reflection::GetType("SteelEngine::Core")->SetMetaData(GlobalSystems::DELTA_TIME, &Reflection::CreateInstance("SteelEngine::DeltaTime")->m_Object);

        m_DeltaTimeVariant = Reflection::GetType("SteelEngine::Core")->GetMetaData(GlobalSystems::DELTA_TIME);

        m_ReflectionGenerator = new ReflectionGenerator();

        if(m_EnginePathVariant == EnginePathVariant::ENGINE_DEV)
        {
            m_RuntimeReloader =
                (HotReloader::RuntimeReloader*)SteelEngine::Reflection::CreateInstance("SteelEngine::HotReloader::RuntimeReloader");

            m_RuntimeReloader->SetReflectionGenerator(m_ReflectionGenerator);

            if(m_RuntimeReloader->Initalize() == SteelEngine::SE_FALSE)
            {
                SE_FATAL("Runtime compiler initialization failed!");

                return SE_FALSE;
            }
        }

        m_NetworkManager = (Network::INetworkManager*)Reflection::CreateInstance("SteelEngine::Network::NetworkManager");

        m_NetworkManager->Init();

        Reflection::GetType("SteelEngine::Core")->SetMetaData(GlobalSystems::NETWORK_MANAGER, &m_NetworkManager->m_Object);

        m_VirtualProject = (IVirtualProject**)&Reflection::CreateInstance("SteelEngine::VirtualProject")->m_Object;

        (*m_VirtualProject)->SetReflectionGenerator(m_ReflectionGenerator);
        (*m_VirtualProject)->Init();

    // Graphics stuff
        m_Window = (IWindow*)Reflection::CreateInstance("SteelEngine::VulkanWindow");
        m_Renderer = (Graphics::IRenderer**)&Reflection::CreateInstance("SteelEngine::Graphics::Vulkan::Renderer", m_Window)->m_Object;
        m_Editor = (Editor::IEditor**)&Reflection::CreateInstance("SteelEngine::Editor::ImGUI::ImGUI_Editor")->m_Object;

        m_Window->SetTitle("Test Window!");
        m_Window->SetWidth(1920);
        m_Window->SetHeight(1080);

        std::function<void(void*, IWindow*)> func = [&](void* event_, IWindow* window)
        {
            SDL_Event* event = (SDL_Event*)event_;

            (*m_Editor)->ProcessEvents(event);

            if(event->type == SDL_QUIT)
            {

            }
            else if(event->type == SDL_WINDOWEVENT)
            {
                switch (event->window.event)
                {
                case SDL_WINDOWEVENT_RESIZED:
                    Event::GlobalEvent::Broadcast(ResizeEvent
                        {
                            (Type::uint32)event->window.data1,
                            (Type::uint32)event->window.data2
                        }
                    );
                    break;
                case SDL_WINDOWEVENT_MINIMIZED:
                    Event::GlobalEvent::Broadcast(MinimizedEvent{});
                    break;
                case SDL_WINDOWEVENT_MAXIMIZED:
                    Event::GlobalEvent::Broadcast(MaximizedEvent{});
                    break;
                case SDL_WINDOWEVENT_RESTORED:
                    Event::GlobalEvent::Broadcast(MaximizedEvent{});
                    break;
                default:
                    break;
                }
            }
            else if(event->type == SDL_KEYDOWN)
            {
                Event::GlobalEvent::Broadcast(KeyDownEvent{ event->key.keysym.scancode });
            }
            else if(event->type == SDL_KEYUP)
            {
                Event::GlobalEvent::Broadcast(KeyUpEvent{ event->key.keysym.scancode });
            }
            else if(event->type == SDL_MOUSEMOTION)
            {
                Event::GlobalEvent::Broadcast(MouseMotionEvent{ event->motion.x, event->motion.y });
            }
        };

        Reflection::GetType("SteelEngine::VulkanWindow")->Invoke("SetProcessEventsCallback", m_Window, func);

        if(m_Window->Create() == SE_FALSE)
        {
            printf("Window creation failed!\n");

            return SE_FALSE;
        }

        Event::GlobalEvent::Add<IWindow::WindowCloseEvent>((Core*)m_Object);

        FileSystem::Map("shaders", getBinaryLocation() / "Resources/Shaders");

        if((*m_Renderer)->Init() == SE_FALSE)
        {
            printf("Renderer init failed!\n");

            return SE_FALSE;
        }

        m_ImGUI_ContextAPI = (IContext*)Reflection::CreateInstance("SteelEngine::VulkanContext");

        if(m_ImGUI_ContextAPI)
        {
            m_ImGUI_ContextAPI->Init(m_Window, *m_Renderer);

            SE_INFO("Editor API context initialization success!");

            (*m_Editor)->SetVirtualProjectVisualizer((*m_VirtualProject)->GetVisualizer());

            if((*m_Editor)->Init(*m_Renderer, m_ImGUI_ContextAPI) == SE_FALSE)
            {
                SE_FATAL("Editor initialization failed!");

                return SE_FALSE;
            }
            else
            {
                SE_INFO("Editor initialization success!");
            }
            
        }
        else
        {
            SE_FATAL("Problem with editor API context!");
        }

    // ---------------------------------------------------------------------

        SE_INFO("Core initialized successful!");

        Event::GlobalEvent::Add<LoadedProjectEvent>(this);

        return SE_TRUE;
    }

    Core::Core()
    {
        m_Running = false;
        m_RuntimeReloader = 0;
    }

    Core::~Core()
    {

    }

    void Core::Update()
    {
        (*m_DeltaTimeVariant->Convert<IDeltaTime**>())->Update();

        // The runtime compiler file watcher is not too perform
        if(m_RuntimeReloader)
        {
            m_RuntimeReloader->m_Object->Update();
        }

        m_Logger->m_Object->Update();
        m_ImGUI_ContextAPI->Update();
        (*m_Editor)->Draw();
        m_Window->m_Object->Update();
        (*m_Renderer)->Update();
        (*m_Renderer)->PreRender();
        (*m_Renderer)->Render();
        m_ImGUI_ContextAPI->UploadDrawData();
        (*m_Renderer)->PostRender();
        (*m_VirtualProject)->Update();
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

        m_Running = false;
    }

    void Core::SetPathVariant(EnginePathVariant variant)
    {
        m_EnginePathVariant = variant;

        Reflection::GetType("SteelEngine::Core")->SetMetaData(ReflectionAttribute::ENGINE_START_TYPE, variant);
    }

    void Core::OnRecompile(HotReloader::IRuntimeObject* oldObject)
    {
        Event::GlobalEvent::Remove<IWindow::WindowCloseEvent>(oldObject);
        Event::GlobalEvent::Add<IWindow::WindowCloseEvent>((Core*)m_Object);
    }

    void Core::operator()(const IWindow::WindowCloseEvent& event)
    {
        Stop();
    }

    void Core::operator()(const LoadedProjectEvent& event)
    {
        if(m_EnginePathVariant == EnginePathVariant::GAME_DEV)
        {
            m_RuntimeReloader =
                (HotReloader::RuntimeReloader*)SteelEngine::Reflection::CreateInstance("SteelEngine::HotReloader::RuntimeReloader");

            m_RuntimeReloader->SetReflectionGenerator(m_ReflectionGenerator);

            if(m_RuntimeReloader->Initalize() == SteelEngine::SE_FALSE)
            {
                SE_FATAL("Runtime Compiler initialization failed!");
            }
        }
    }

    void Core::operator()(GetCompileConfigEvent* event)
    {
        event->m_Config = m_CompileConfig;
    }

}