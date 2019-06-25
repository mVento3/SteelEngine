#pragma once

#include "Core/ICore.h"

#include "Networking/INetwork.h"

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/Reflection.h"
#include "RuntimeReflection/ReflectionAttributes.h"

#include "Core.Generated.h"

#include "RuntimeCompiler/RuntimeCompiler.h"
#include "RuntimeCompiler/Serializer.h"

#include "Utils/Utils.h"
#include "Utils/Time.h"

#include "Core/Server.h"

#include "Logger/Logger.h"

#include "Rendering/IRenderer.h"

#include "FileSystem/FileSystem.h"

#include "Window/IWindow.h"

// 1. Fix problem with properties in proper class, especially while nesting

namespace SteelEngine {

    SE_CLASS(SteelEngine::ReflectionAttribute::SE_RUNTIME_SERIALIZE)
    class Core : public Interface::ICore
    {
        GENERATED_BODY
    public:
        enum EnginePathVariant
        {
            ENGINE_DEV,
            GAME_DEV
        };

        enum EngineInfo
        {
            IS_SERVER,
            SERVER_IP
        };

    private:
        RuntimeCompiler*        m_RuntimeCompiler;
        Interface::INetwork*    m_Network;
        Interface::ILogger*     m_Logger;
        Interface::IRenderer*   m_Renderer;
        Interface::IWindow*     m_Window;

        bool m_Running;

        EnginePathVariant m_EnginePathVariant;

        Variant* meta;
        Variant* meta2;
        Variant* meta3;

        float m_Delta;
        Type::uint32 m_Frames;

        void Loop();
        Result Init();
        void Cleanup();

    public:
        Core();
        ~Core();

        SE_VALUE(
            "lol" = 1212,
            SteelEngine::ReflectionAttribute::SE_RUNTIME_SERIALIZE = 11
        )
        bool ta;

        void Update() override;

        SE_METHOD()
        void Start() override;

        SE_METHOD()
        void Stop() override;

        SE_METHOD()
        void SetPathVariant(EnginePathVariant variant);

        void operator()(const Interface::IWindow::WindowCloseEvent& event);
    };

}