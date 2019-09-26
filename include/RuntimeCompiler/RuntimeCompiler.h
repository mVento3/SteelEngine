#pragma once

#include "RuntimeCompiler/IRuntimeCompiler.h"
#include "RuntimeCompiler/Serializer.h"
#include "RuntimeCompiler/SwapModuleEvent.h"
#include "RuntimeCompiler/RuntimeCompiler.Generated.h"

#include "RuntimeCompiler/Events/StopRecompilingEvent.h"
#include "RuntimeCompiler/Events/StartRecompilingEvent.h"

#include "windows.h"
#include "string"

#include "FileWatcher/FileWatcher.h"

#include "RuntimeReflection/IReflectionGenerator.h"
#include "RuntimeReflection/Macro.h"

#include "Networking/Config.h"

#include "PythonProcess/IPythonProcess.h"

#include "Core/GetCompileConfigEvent.h"

namespace SteelEngine { namespace HotReload {

    SE_CLASS()
    class RuntimeCompiler : public IRuntimeCompiler
    {
        GENERATED_BODY
    public:
        typedef IPythonProcess*(*ProcessAllocator)();

        struct Thread
        {
            Thread()
            {
                m_Done = false;
            }

            std::thread m_Thread;
            bool m_Done;
        };

        struct HotModule
        {
            void*       m_Module;
            std::string m_ModuleName;
        };

        const static std::string cs_CompletionToken;

    private:
        FileWatcher*                    m_SourceFileWatcher;
        IReflectionGenerator*           m_ReflectionGenerator;
        SteelEngine::IPythonProcess*    m_Process;

        std::filesystem::path m_BinaryLocation;
        Utils::json m_CompileConfig;
        bool m_Once;
        bool m_IsSwapComplete;
        bool m_Paused = false;
        bool m_Initialized = false;

        std::vector<Thread*> m_Threads;
        std::vector<HotModule> m_LoadedHotModules;
        std::vector<std::string> m_ObjFilesToDelete;

        std::string m_ModuleName;

        void GenerateModuleName();

    public:
        RuntimeCompiler();
        ~RuntimeCompiler();

        Result Initalize() override;
        void Cleanup() override;

        void Update() override;
        void Compile(const std::filesystem::path& file) override;
        void SwapModule(const std::string& moduleName) override;

        void operator()(const SwapModuleEvent& event);

        void operator()(const StopRecompilingEvent& event);
        void operator()(const StartRecompilingEvent& event);

        inline Result IsCompileComplete() override { return m_Process->IsCompileComplete(); }
        inline Result IsSwapComplete() override { return m_IsSwapComplete; }

        void SetReflectionGenerator(IReflectionGenerator* reflectionGenerator) override { m_ReflectionGenerator = reflectionGenerator; }
    };

}}