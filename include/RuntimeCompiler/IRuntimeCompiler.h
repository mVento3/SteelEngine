#pragma once

#include "filesystem"

#include "Core/Result.h"

#include "RuntimeCompiler/IRuntimeObject.h"

#include "RuntimeReflection/IReflectionGenerator.h"

namespace SteelEngine { namespace HotReload {

    struct IRuntimeCompiler : public IRuntimeObject
    {
        virtual Result Initalize() { return SE_NOT_IMPLEMENTED; }
        virtual void Cleanup() { }

        virtual void Update() { }
        virtual void Compile(const std::filesystem::path& file) { }
        virtual void SwapModule(const std::string& moduleName) { }

        virtual inline Result IsCompileComplete() { return SE_NOT_IMPLEMENTED; }
        virtual inline Result IsSwapComplete() { return SE_NOT_IMPLEMENTED; }

        virtual void SetReflectionGenerator(IReflectionGenerator* reflectionGenerator) { }
    };

}}