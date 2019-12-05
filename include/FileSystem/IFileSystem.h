#pragma once

#include "HotReloader/IRuntimeObject.h"

#include "filesystem"

namespace SteelEngine {

    struct IFileSystem : public HotReloader::IRuntimeObject
    {
    protected:

    public:
        virtual void MapImpl(const std::string& name, const std::filesystem::path& dir) { }
        virtual const std::filesystem::path& GetImpl(const std::string& name) { return "NONE"; }
    };

}