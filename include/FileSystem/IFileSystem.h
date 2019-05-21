#pragma once

#include "RuntimeCompiler/IRuntimeObject.h"

#include "Core/Platform.h"

namespace SteelEngine { namespace Interface {

    struct IFileSystem : public IRuntimeObject
    {
        virtual void MapImpl(const std::string& name, const filesystem::path& dir) { }
        virtual const filesystem::path& GetImpl(const std::string& name) { return "NONE"; }
    };

}}