#pragma once

#include "filesystem"

#include "Core/Result.h"

#include "HotReloader/IRuntimeObject.h"

#include "RuntimeReflection/IReflectionGenerator.h"

namespace SteelEngine { namespace HotReloader {

    struct IRuntimeReloader
    {
        virtual Result Initalize()
        {
            return SE_NOT_IMPLEMENTED;
        }

        virtual void Cleanup()
        {

        }

        virtual void Update()
        {

        }

        virtual void Compile(const std::filesystem::path& file)
        {

        }

        virtual void SwapModule(const std::string& moduleName)
        {

        }

        virtual inline Result IsCompileComplete()
        {
            return SE_NOT_IMPLEMENTED;
        }

        virtual inline Result IsSwapComplete()
        {
            return SE_NOT_IMPLEMENTED;
        }

        virtual void SetReflectionGenerator(IReflectionGenerator* reflectionGenerator)
        {

        }
    };

}}