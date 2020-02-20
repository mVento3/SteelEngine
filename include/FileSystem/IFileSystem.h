#pragma once

#include "filesystem"

namespace SteelEngine {

    struct IFileSystem
    {
    protected:

    public:
        virtual void MapImpl(const std::string& name, const std::filesystem::path& dir) { }
        virtual const std::filesystem::path& GetImpl(const std::string& name) { return "NONE"; }
    };

}