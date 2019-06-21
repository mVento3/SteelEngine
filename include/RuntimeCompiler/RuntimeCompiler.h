#pragma once

#include "RuntimeCompiler/IRuntimeCompiler.h"
#include "RuntimeCompiler/Serializer.h"
#include "RuntimeCompiler/SwapModuleEvent.h"

#include "windows.h"
#include "string"

#include "FileWatcher/FileWatcher.h"

#include "RuntimeReflection/ReflectionGenerator.h"
#include "RuntimeReflection/Macro.h"

#include "Networking/SwapModuleNetCommandEvent.h"
#include "Networking/Config.h"

#include "RuntimeCompiler.Generated.h"

namespace SteelEngine {

    SE_CLASS()
    class RuntimeCompiler : public Interface::IRuntimeCompiler
    {
        GENERATED_BODY
    public:
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
        FileWatcher*            m_SourceFileWatcher;
        ReflectionGenerator*    m_ReflectionGenerator;

        filesystem::path m_BinaryLocation;

        std::vector<Thread*> m_Threads;
        std::vector<HotModule> m_LoadedHotModules;

        std::string m_ModuleName;
        std::string m_SourceFile;

        std::vector<std::string> m_AdditionalDependencies;

        Result SetupProcess();

        void GenerateModuleName();

    public:
        RuntimeCompiler();
        ~RuntimeCompiler();

        bool m_IsCompileComplete;
        bool m_IsSwapComplete;
        bool m_Error;

        HANDLE m_CmdProcessOutputRead;
		HANDLE m_CmdProcessInputWrite;

        PROCESS_INFORMATION m_CmdProcessInfo;

        Result Initalize() override;
        void Cleanup() override;

        void Update() override;
        void Compile(const filesystem::path& file) override;
        void SwapModule(const std::string& moduleName) override;

        void WriteInput(const std::string& input);

        void operator()(const SwapModuleEvent& event);

        inline Result IsCompileComplete() override { return m_IsCompileComplete; }
        inline Result IsSwapComplete() override { return m_IsSwapComplete; }
    };

}