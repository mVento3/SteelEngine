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

#include "Event/EventManager.h"

#include "entt/entt.hpp"

#include "Graphics/Model.h"

#include "Graphics/ECS_Components/TransformComponent.h"

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
        EventManager aa;
        Reflection aaa;

        aaa.Init2();

        m_EventManager = (IEventManager**)&Reflection::CreateInstance("SteelEngine::EventManager")->m_Object;

        Reflection::GetType("SteelEngine::Core")->SetMetaData(GlobalSystems::EVENT_MANAGER, m_EventManager);
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

        std::ifstream configFile(getBinaryLocation() / "config.json");

        configFile >> m_CompileConfig;
        Reflection::GetType("SteelEngine::Core")->SetMetaData(Reflection::ReflectionAttribute::PROJECTS_PATH, (std::string)m_CompileConfig["projects_path"]);

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
            Graphics::IRenderer::OPENGL_API
        );

        std::vector<SteelEngine::IReflectionData*> types =
            Reflection::GetTypesByMetaData(Reflection::ReflectionAttribute::REFLECTION_MODULE, [](Variant* key) -> bool
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
        m_Window = (IWindow*)Reflection::CreateInstance("SteelEngine::OpenGL_Window");
        m_Renderer = (Graphics::IRenderer**)&Reflection::CreateInstance("SteelEngine::Graphics::OpenGL::Renderer", m_Window)->m_Object;
        m_Editor = (Editor::IEditor**)&Reflection::CreateInstance("SteelEngine::Editor::ImGUI::ImGUI_Editor")->m_Object;

        m_SceneManager = (ISceneManager**)&Reflection::CreateInstance("SteelEngine::SceneManager")->m_Object;

        // entt::registry* reg = (*m_SceneManager)->GetCurrentScene();

        // auto ent = reg->create();

        (*m_SceneManager)->Init();

        EventObserver* eve = Reflection::GetType("SteelEngine::Graphics::OpenGL::Renderer")->Invoke("Cast_EventObserver", *m_Renderer).Convert<EventObserver*>();
        EventObserver* eve2 = Reflection::GetType("SteelEngine::Editor::ImGUI::ImGUI_Editor")->Invoke("Cast_EventObserver", *m_Editor).Convert<EventObserver*>();

        (*m_EventManager)->AddEventObserver(eve);
        (*m_EventManager)->AddEventObserver(eve2);

        m_Window->SetTitle("Test Window!");
        m_Window->SetWidth(1920);
        m_Window->SetHeight(1080);

        std::function<void(void*, IWindow*)> func = [&](void* event_, IWindow* window)
        {
            SDL_Event* event = (SDL_Event*)event_;

            (*m_Editor)->ProcessEvents(event);
        };

        Reflection::GetType("SteelEngine::OpenGL_Window")->Invoke("SetProcessEventsCallback", m_Window, func);

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

        m_ImGUI_ContextAPI = (IContext*)Reflection::CreateInstance("SteelEngine::OpenGL_Context");

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

            Reflection::GetType("SteelEngine::OpenGL_Context")->Invoke("MakeCurrent", m_ImGUI_ContextAPI);
        }
        else
        {
            SE_FATAL("Problem with editor API context!");
        }

    // ---------------------------------------------------------------------

        SE_INFO("Core initialized successful!");

        Event::GlobalEvent::Add<LoadedProjectEvent>(this);

        ent = (*m_Renderer)->AddModel
            (Graphics::Model::Create("D:/Projects/C++/SteelEngine/bin/Resources/Models/test.obj"),
            (*m_SceneManager)->GetCurrentScene(),
            Transform(glm::vec3(0, 20, 10))
        );

        for(Type::uint32 i = 0; i < 50; i++)
        {
            (*m_Renderer)->AddModel
                (Graphics::Model::Create("D:/Projects/C++/SteelEngine/bin/Resources/Models/cube.obj"),
                (*m_SceneManager)->GetCurrentScene(),
                Transform(glm::vec3(i * 2, 1, 0))
            );
        }

        for(Type::uint32 i = 0; i < 10; i++)
        {
            for(Type::uint32 j = 0; j < 30; j++)
            {
                (*m_Renderer)->AddModel
                    (Graphics::Model::Create("D:/Projects/C++/SteelEngine/bin/Resources/Models/a.obj"),
                    (*m_SceneManager)->GetCurrentScene(),
                    Transform(glm::vec3(j * 2, (i * 2) + 5, 0))
                );
            }
        }

        Transform trans;

        trans.SetScale(glm::vec3(100, 100, 100));

        (*m_Renderer)->AddModel
            (Graphics::Model::Create("D:/Projects/C++/SteelEngine/bin/Resources/Models/test.obj"),
            (*m_SceneManager)->GetCurrentScene(),
            trans
        );

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
        IDeltaTime* time = (*m_DeltaTimeVariant->Convert<IDeltaTime**>());

        time->Update();

        {
            SE_PROFILE_SCOPE("Naive Event Manager");

            ProcessEvents(*m_EventManager);
        }

        // The runtime compiler file watcher is not too perform
        if(m_RuntimeReloader)
        {
            m_RuntimeReloader->m_Object->Update();
        }

        m_OneSecondTime += time->GetDeltaTime();

        if(m_OneSecondTime >= 1)
        {
            // printf("FPS: %d\n", time->GetUPS());

            m_OneSecondTime = 0;
        }

        {
            SE_PROFILE_SCOPE("Logger");

            m_Logger->m_Object->Update();
        }

        {
            SE_PROFILE_SCOPE("Editor");

            m_ImGUI_ContextAPI->Update();
            (*m_Editor)->Draw();
        }
    
        m_Window->m_Object->Update();

        {
            SE_PROFILE_SCOPE("Graphics");

            {
                SE_PROFILE_SCOPE("Graphics::Update");

                (*m_Renderer)->Update();
            }

            {
                SE_PROFILE_SCOPE("Graphics::PreRender");

                (*m_Renderer)->PreRender();
            }

            {
                SE_PROFILE_SCOPE("Graphics::Render");

                (*m_Renderer)->Render((*m_SceneManager)->GetCurrentScene());
            }

            m_ImGUI_ContextAPI->UploadDrawData();

            {
                SE_PROFILE_SCOPE("Graphics::PostRender");

                (*m_Renderer)->PostRender();
            }
        }

        {
            SE_PROFILE_SCOPE("Project");

            (*m_VirtualProject)->Update();
        }
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

        Reflection::GetType("SteelEngine::Core")->SetMetaData(Reflection::ReflectionAttribute::ENGINE_START_TYPE, variant);
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