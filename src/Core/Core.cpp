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
#include "Graphics/IndexBuffer.h"
#include "Graphics/VertexBuffer.h"

#include "AssetManager/AssetManager.h"

#include "AssetManager/Events/LoadAssetEvent.h"

#include "Graphics/ECS_Components/TransformComponent.h"

#include "Editor/ImGUI/ImGUI_Editor.h"

#include "Platform/Graphics/RenderDevice.h"

namespace SteelEngine {

    void Core::Loop()
    {
        while(m_Running)
        {
            Update();
        }

        m_RenderContext->GetCurrentRenderer()->Cleanup();
        m_Window->Close();

        Cleanup();
    }

    void Core::Cleanup()
    {

    }

    Result Core::Init()
    {
        SE_PROFILE_FUNC;

        m_EventManager = (IEventManager*)Reflection::CreateInstance("SteelEngine::EventManager");

        Reflection::GetType("SteelEngine::Core")->SetMetaData(GlobalSystems::EVENT_MANAGER, m_EventManager);
        Reflection::GetType("SteelEngine::Core")->SetMetaData(GlobalSystems::PROFILER, Reflection::CreateInstance_("SteelEngine::Profiler::Manager"));

        std::string logPath = FileSystem::Get("se_init_log").string();

        m_Logger = (ILogger*)Reflection::CreateInstance("SteelEngine::Logger", logPath.c_str());

        if(!m_Logger || m_Logger->Init() == SE_FALSE)
        {
            printf("Failed to setup logger!\n");

            return SE_FALSE;
        }

        Reflection::GetType("SteelEngine::Core")->SetMetaData(GlobalSystems::LOGGER, m_Logger);

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

        std::vector<SteelEngine::IReflectionData*> types =
            Reflection::GetTypesByMetaData(Reflection::ReflectionAttribute::REFLECTION_MODULE, [](Variant* key) -> bool
            {
                return key->Convert<bool>();
            });

        for(Type::uint32 i = 0; i < types.size(); i++)
        {
            m_ReflectionModules.push_back(types[i]->Create());
        }

        Reflection::GetType<Core>()->SetMetaData(GlobalSystems::DELTA_TIME, Reflection::CreateInstance("SteelEngine::DeltaTime"));

        m_DeltaTimeVariant = Reflection::GetType("SteelEngine::Core")->GetMetaData(GlobalSystems::DELTA_TIME);

        m_ReflectionGenerator = (SteelEngine::IReflectionGenerator*)SteelEngine::Reflection::CreateInstance("SteelEngine::ReflectionGenerator");

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

        Reflection::GetType("SteelEngine::Core")->SetMetaData(GlobalSystems::NETWORK_MANAGER, m_NetworkManager);

        m_VirtualProject = (IVirtualProject**)Reflection::CreateInstance_("SteelEngine::VirtualProject");

        (*m_VirtualProject)->SetReflectionGenerator(m_ReflectionGenerator);
        (*m_VirtualProject)->Init();

    // Graphics stuff
        m_RenderContext = Reflection::CreateInstance<Utils::RenderContext>();
        m_SceneManager = (ISceneManager*)Reflection::CreateInstance("SteelEngine::SceneManager");

        SetScene(m_RenderContext, m_SceneManager);

        m_SceneManager->Init();
        m_RenderContext->Initialize();
        m_RenderContext->SelectPlatform(Graphics::IRenderer::API::OPENGL);
        m_RenderContext->SelectRenderer("SteelEngine::Graphics::OpenGL::Renderer");

        m_Editor = Reflection::CreateInstance<Editor::ImGUI::ImGUI_Editor>();

        std::function<void(void*, IWindow*)> func = [&](void* event_, IWindow* window)
        {
            SDL_Event* event = (SDL_Event*)event_;

            m_Editor->ProcessEvents(event);
        };

        m_Window = Window::Create(Window::WindowAPI::SDL);

        m_Window->SetProcessEventsCallback(func);
        m_Window->SetTitle("Test Window!");
        m_Window->SetWidth(1920);
        m_Window->SetHeight(1080);

        if(m_Window->Create() == SE_FALSE)
        {
            printf("Window creation failed!\n");

            return SE_FALSE;
        }

        m_RenderContext->GetCurrentRenderer()->SetRenderTarget(m_Window);
        m_RenderContext->GetCurrentRenderer()->SetRenderContext(m_RenderContext);

        uint32_t extensionsCount;
        const char** extensions;

        m_Window->GetVulkanInstanceExtensions(&extensionsCount, NULL);

        if(extensionsCount)
        {
            extensions = new const char*[extensionsCount];
            m_Window->GetVulkanInstanceExtensions(&extensionsCount, extensions);
        }

        m_RenderContext->GetRenderDevice()->SetWindow(m_Window);
        m_RenderContext->GetRenderDevice()->SetVulkanExtensions(extensionsCount, extensions);
        m_RenderContext->GetRenderDevice()->Initialize();
        m_RenderContext->GetRenderDevice()->SetEditorCommands(m_Editor->GetEditorCommands());
        m_RenderContext->GetRenderDevice()->SetUpdate(m_Editor->GetUpdate());
        m_Editor->SetVirtualProjectVisualizer((*m_VirtualProject)->GetVisualizer());
        m_Editor->Init(m_RenderContext, m_Window);
        m_RenderContext->GetRenderDevice()->PostInitialize();

        if(m_RenderContext->GetCurrentRenderer()->Init() == SE_FALSE)
        {
            printf("Renderer init failed!\n");

            return SE_FALSE;
        }

    // TODO: IDK about that, i mean event system
        m_EventManager->AddEventObserver(
            Reflection::GetType("SteelEngine::Editor::ImGUI::ImGUI_Editor")->Invoke("Cast_EventObserver", m_Editor).Convert<EventObserver*>()
        );

        Event::GlobalEvent::Add<IWindow::WindowCloseEvent>(this);

        FileSystem::Map("shaders", getBinaryLocation() / "Resources/Shaders");

        m_AssetManager = (IAssetManager*)Reflection::CreateInstance<AssetManager>();

        Reflection::GetType<Core>()->SetMetaData(GlobalSystems::ASSET_MANAGER, m_AssetManager);

    // ---------------------------------------------------------------------

        SE_INFO("Core initialized successful!");

        Event::GlobalEvent::Add<LoadedProjectEvent>(this);

        LoadAssetEvent loadAsset;

        loadAsset.m_Path = "D:/Projects/C++/SteelEngine/bin/Resources/Models/test.obj";

        Event::GlobalEvent::Broadcast_(&loadAsset);

        Transform trans;

        trans.SetScale(glm::vec3(100, 100, 100));

        m_RenderContext->GetCurrentRenderer()->AddModel(
            ((AssetItem<Graphics::IModel>*)loadAsset.m_Result)->GetAsset(),
            m_SceneManager,
            trans
        );

        Event::GlobalEvent::Broadcast_(&loadAsset);

        trans.SetPosition(glm::vec3(0, -10, 0));

        m_RenderContext->GetCurrentRenderer()->AddModel(
            ((AssetItem<Graphics::IModel>*)loadAsset.m_Result)->GetAsset(),
            m_SceneManager,
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
        IDeltaTime* time = m_DeltaTimeVariant->Convert<IDeltaTime*>();

        time->Update();

        {
            SE_PROFILE_SCOPE("Naive Event Manager");

            ProcessEvents(m_EventManager);
        }

    // The runtime compiler file watcher is not too perform
        if(m_RuntimeReloader)
        {
            m_RuntimeReloader->Update();
        }

        {
            SE_PROFILE_SCOPE("Project");

            (*m_VirtualProject)->Update();
        }

        m_OneSecondTime += time->GetDeltaTime();

        if(m_OneSecondTime >= 1)
        {
            // printf("FPS: %d\n", time->GetUPS());

            m_OneSecondTime = 0;
        }

        {
            SE_PROFILE_SCOPE("Logger");

            m_Logger->Update();
        }

        m_Window->Update();

        {
            SE_PROFILE_SCOPE("Editor");

            m_Editor->Draw(m_RenderContext->GetCurrentRenderer()->GetFinalTexture());
        }

        {
            SE_PROFILE_SCOPE("Graphics");

            Graphics::IRenderer* renderer = m_RenderContext->GetCurrentRenderer();

            renderer->Update();
            renderer->PreRender();
            renderer->Render(m_SceneManager->GetCurrentScene());
            renderer->PostRender();
        }

        m_Window->SwapBuffers();
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

    // void Core::OnRecompile(HotReloader::IRuntimeObject* oldObject)
    // {
    //     Event::GlobalEvent::Remove<IWindow::WindowCloseEvent>(oldObject);
    //     Event::GlobalEvent::Add<IWindow::WindowCloseEvent>((Core*)m_Object);
    // }

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