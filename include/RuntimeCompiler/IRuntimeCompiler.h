#pragma once

#include "HotReloader/IRuntimeObject.h"

#include "Subprocess/ISubprocess.h"

#include "Utils/Json.h"

#include "string"
#include "vector"

namespace SteelEngine {

    struct IRuntimeCompiler : public HotReloader::IRuntimeObject
    {
        virtual void Setup() = 0;
        virtual void Compile(
            const std::string& source,
            const std::vector<std::string>& flags,
            const std::vector<std::string>& defines,
            const std::vector<std::string>& includes,
            const std::vector<std::string>& libPaths,
            const std::vector<std::string>& libs
        ) = 0;
        virtual void Compile(
            const std::string& source,
            const Utils::json& flags,
            const Utils::json& definitions,
            const Utils::json& includes,
            const Utils::json& libPaths,
            const Utils::json& libs
        ) = 0;
        // The options should contain:
        // - "flags" string array
        // - "definitions" string array
        // - "includes" string array
        // - "lib_paths" string array
        // - "libs" string array
        virtual void Compile(
            const std::string& source,
            const Utils::json& options
        ) = 0;

        virtual void WaitUntilComplete() const = 0;
        virtual bool IsCompileComplete() const = 0;
        virtual bool WasError() const = 0;
        virtual const std::string& GetErrorMessage() const = 0;
    };

}