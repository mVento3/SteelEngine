#pragma once

#include "RuntimeCompiler/IRuntimeCompiler.h"

#include "Subprocess/ISubprocess.h"

#include "RuntimeCompiler/RuntimeCompiler.Generated.h"

namespace SteelEngine {

    SE_CLASS()
    class RuntimeCompiler : public IRuntimeCompiler
    {
        GENERATED_BODY
    private:
        ISubprocess* m_Process;

        static const std::string mcs_CompletionToken;

    public:
        RuntimeCompiler();
        RuntimeCompiler(ISubprocess* process);
        ~RuntimeCompiler();

        void Setup() override;
        void Compile(
            const std::string& source,
            const std::vector<std::string>& flags,
            const std::vector<std::string>& defines,
            const std::vector<std::string>& includes,
            const std::vector<std::string>& libPaths,
            const std::vector<std::string>& libs
        ) override;
        void Compile(
            const std::string& source,
            const Utils::json& flags,
            const Utils::json& definitions,
            const Utils::json& includes,
            const Utils::json& libPaths,
            const Utils::json& libs
        ) override;
        void Compile(
            const std::string& source,
            const Utils::json& options
        ) override;

        void WaitUntilComplete() const override;

        inline bool IsCompileComplete() const override
        {
            return m_Process->IsCompileComplete();
        }

        inline bool WasError() const
        {
            return m_Process->WasError();
        }

        inline const std::string& GetErrorMessage() const
        {
            return m_Process->GetErrorMessage();
        }
    };

}