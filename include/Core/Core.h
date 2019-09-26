#pragma once

#include "FileSystem/FileSystem.h"

#include "Networking/INetwork.h"
#include "Networking/INetworkManager.h"

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/Reflection.h"

#include "RuntimeCompiler/RuntimeCompiler.h"
#include "RuntimeCompiler/Serializer.h"

#include "Utils/Utils.h"
#include "Utils/Time.h"

#include "Logger/Logger.h"

#include "Graphics/IRenderer.h"

#include "Window/IWindow.h"
#include "Window/ResizeEvent.h"
#include "Window/MinimizedEvent.h"
#include "Window/MaximizedEvent.h"

#include "Graphics/IEditor.h"
#include "Graphics/Renderer.h"

#include "VirtualProject/IVirtualProject.h"
#include "VirtualProject/LoadedProjectEvent.h"

#include "PythonCore/IPythonCore.h"

#include "nlohmann/json.hpp"

#include "ImGUI_Editor/IContext.h"

#include "Core/ICore.h"
#include "Core/ReflectionAttributes.h"
#include "Core/Core.Generated.h"
#include "Core/GetCompileConfigEvent.h"

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

        // enum EngineInfo
        // {
        //     IS_SERVER,
        //     SERVER_IP
        // };

        enum GlobalSystems
        {
            PYTHON
        };

    private:
        HotReload::RuntimeCompiler* m_RuntimeCompiler;
        IReflectionGenerator*       m_ReflectionGenerator;
        Interface::ILogger*         m_Logger;
        Graphics::IRenderer**       m_Renderer;
        IWindow*                    m_Window;
        Editor::IEditor**           m_Editor;
        IVirtualProject**           m_VirtualProject;
        Network::INetworkManager*   m_NetworkManager;
        Script::IPython*            m_Python;
        IContext*                   m_ImGUI_ContextAPI;

        json m_CompileConfig;

        bool m_Running;

        EnginePathVariant m_EnginePathVariant;

        float m_Delta;
        Type::uint32 m_Frames;

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

        void operator()(const IWindow::WindowCloseEvent& event);
        void operator()(const LoadedProjectEvent& event);
        void operator()(GetCompileConfigEvent* event);
    };

}