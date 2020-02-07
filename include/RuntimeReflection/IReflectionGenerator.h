#pragma once

#include "filesystem"

#include "Core/Type.h"

#include "HotReloader/IRuntimeObject.h"

namespace SteelEngine {

    struct IReflectionGenerator : public HotReloader::IRuntimeObject
    {
        virtual Result Load(const std::filesystem::path& filename) = 0;
        virtual Result Parse() = 0;
        virtual Result Generate(const std::filesystem::path& cwd, const std::filesystem::path& generatePath) = 0;
        virtual void Clear() = 0;
    };

}