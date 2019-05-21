#pragma once

#include "filesystem"

#include "Core/Result.h"
#include "Core/Platform.h"

#include "RuntimeCompiler/IRuntimeObject.h"

namespace SteelEngine { namespace Interface {

    struct IRuntimeCompiler : public IRuntimeObject
    {
        virtual Result Initalize() { return SE_NOT_IMPLEMENTED; }
        virtual void Cleanup() { }

        virtual void Update() { }
        virtual void Compile(const filesystem::path& file) { }
        virtual void SwapModule(const std::string& moduleName) { }

        virtual inline Result IsCompileComplete() { return SE_NOT_IMPLEMENTED; }
        virtual inline Result IsSwapComplete() { return SE_NOT_IMPLEMENTED; }
    };

}}