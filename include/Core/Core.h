#pragma once

#include "Networking/INetwork.h"
#include "Networking/INetworkManager.h"

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/Reflection.h"

#include "HotReloader/RuntimeReloader.h"
#include "HotReloader/Serializer.h"

#include "Utils/Utils.h"
#include "Utils/Time.h"

#include "Logger/Logger.h"

#include "Graphics/IRenderer.h"
#include "Graphics/IEditor.h"
#include "Graphics/Renderer.h"
#include "Graphics/IContext.h"

#include "Window/IWindow.h"
#include "Window/ResizeEvent.h"
#include "Window/MinimizedEvent.h"
#include "Window/MaximizedEvent.h"

#include "VirtualProject/IVirtualProject.h"
#include "VirtualProject/LoadedProjectEvent.h"

#include "PythonCore/IPythonCore.h"

#include "nlohmann/json.hpp"

#include "Input/Events/KeyDownEvent.h"
#include "Input/Events/KeyUpEvent.h"
#include "Input/Events/MouseMotionEvent.h"

#include "Core/ICore.h"
#include "Core/ReflectionAttributes.h"
#include "Core/GetCompileConfigEvent.h"
#include "Core/IDeltaTime.h"

#include "Core/Core.Generated.h"

// 1. Fix problem with properties in proper class, especially while nesting
// 2. Replace ImGUI_Program from vulkan to editor module, to do this i need create good render api

namespace SteelEngine {

    using json = nlohmann::json;

    SE_CLASS(
        SteelEngine::ReflectionAttribute::RUNTIME_SERIALIZE
    )
    class Core : public ICore
    {
        friend class PythonCore;
        GENERATED_BODY
    public:
        enum EnginePathVariant
        {
            ENGINE_DEV,
            GAME_DEV
        };

        enum GlobalSystems
        {
            PYTHON,
            LOGGER,
            FILE_SYSTEM,
            DELTA_TIME,
            GLOBAL_EVENTS,
            MEMORY_TRACKER,
            NETWORK_MANAGER
        };

    private:
        HotReloader::RuntimeReloader* m_RuntimeReloader;
        IReflectionGenerator*       m_ReflectionGenerator;
        ILogger*                    m_Logger;
        Graphics::IRenderer**       m_Renderer;
        IWindow*                    m_Window;
        Editor::IEditor**           m_Editor;
        IVirtualProject**           m_VirtualProject;
        Network::INetworkManager*   m_NetworkManager;
        Script::IPython*            m_Python;
        IContext*                   m_ImGUI_ContextAPI;

        Variant* m_DeltaTimeVariant;

        json m_CompileConfig;

        bool m_Running;

        EnginePathVariant m_EnginePathVariant;

        std::vector<HotReloader::IRuntimeObject*> m_ReflectionModules;

        void Loop();
        Result Init();
        void Cleanup();

    public:
        Core();
        ~Core();

        void Update() override;

        SE_METHOD()
        void Start() override;

        SE_METHOD()
        void Stop() override;

        SE_METHOD()
        void SetPathVariant(EnginePathVariant variant);

        void OnRecompile(HotReloader::IRuntimeObject* oldObject) override;

        void operator()(const IWindow::WindowCloseEvent& event);
        void operator()(const LoadedProjectEvent& event);
        void operator()(GetCompileConfigEvent* event);
    };

}