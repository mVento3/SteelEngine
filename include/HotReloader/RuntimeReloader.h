#pragma once

#include "HotReloader/IRuntimeReloader.h"
#include "HotReloader/Serializer.h"

#include "HotReloader/Events/StopRecompilingEvent.h"
#include "HotReloader/Events/StartRecompilingEvent.h"
#include "HotReloader/Events/SwapModuleEvent.h"
#include "HotReloader/Events/RecompiledEvent.h"

#include "RuntimeCompiler/IRuntimeCompiler.h"

#include "windows.h"
#include "string"

#include "FileWatcher/FileWatcher.h"

#include "RuntimeReflection/IReflectionGenerator.h"
#include "RuntimeReflection/Macro.h"

#include "Networking/Config.h"

#include "PythonProcess/IPythonProcess.h"

#include "Core/GetCompileConfigEvent.h"

#include "HotReloader/RuntimeReloader.Generated.h"

namespace SteelEngine { namespace HotReloader {

    SE_CLASS()
    class RuntimeReloader : public IRuntimeReloader
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

        const static std::string mcs_CompletionToken;

    private:
        FileWatcher*                    m_SourceFileWatcher;
        IReflectionGenerator*           m_ReflectionGenerator;
        IPythonProcess*                 m_Process;
        IRuntimeCompiler*               m_Compiler;

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
        RuntimeReloader();
        ~RuntimeReloader();

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