#pragma once

#include "RuntimeCompiler/IRuntimeObject.h"

#include "filesystem"

namespace SteelEngine {

    struct IFileSystem : public HotReload::IRuntimeObject
    {
    protected:

    public:
        virtual void MapImpl(const std::string& name, const std::filesystem::path& dir) { }
        virtual const std::filesystem::path& GetImpl(const std::string& name) { return "NONE"; }
    };

}