#pragma once

#include "Core/ICore.h"

#include "Networking/INetwork.h"

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/Reflection.h"
#include "RuntimeReflection/ReflectionAttributes.h"

#include "Core.Generated.h"

#include "RuntimeCompiler/RuntimeCompiler.h"

#include "Utils/Utils.h"

#include "Core/Server.h"

#include "Logger/Logger.h"

namespace SteelEngine {

    // struct CheckCurrentTextEvent
		// {
		// 	const std::string m_Text;
		// 	const Type::uint32 m_Line;
		// 	const std::string m_FullLine;
		// };

    SE_CLASS(SteelEngine::ReflectionAttribute::SE_RUNTIME_SERIALIZE)
    class Core : public Interface::ICore
    {
        GENERATED_BODY
    public:
        enum Lol
        {
            TES
        };

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
        SteelEngine::RuntimeCompiler* m_RuntimeCompiler;
        Interface::INetwork* m_Network;
        Interface::ILogger* m_Logger;

        bool m_Running;

        EnginePathVariant m_EnginePathVariant;

        Variant* meta;
        Variant* meta2;
        Variant* meta3;

        void Loop();
        Result Init();

    public:
        Core();
        ~Core();

        SE_VALUE("lol" = 1212, SteelEngine::ReflectionAttribute::SE_RUNTIME_SERIALIZE = 11, SteelEngine::Core::Lol::TES = 22)
        bool ta;

        void Update() override;

        SE_METHOD()
        void Start() override;

        SE_METHOD()
        void Stop() override;

        SE_METHOD()
        void SetPathVariant(EnginePathVariant variant);
    };

}