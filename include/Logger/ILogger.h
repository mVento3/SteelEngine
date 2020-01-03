#pragma once

#include "HotReloader/IRuntimeObject.h"

#include "Logger/LogDispatcher.h"

#include "Core/Type.h"
#include "Core/Result.h"

#include "string"
#include "filesystem"

namespace SteelEngine {

    struct ILogger : public HotReloader::IRuntimeObject
    {
        virtual Result Init() { return SE_NOT_IMPLEMENTED; }
        virtual void Log(const std::string& message, int verbosity, Type::uint32 line, const std::filesystem::path& file, va_list args) { }
        virtual void Log(const std::string& message, int verbosity, va_list args) { }
        virtual void Update() override { }
        virtual void SetDispatcher(HotReloader::IRuntimeObject** dispatcher) { }
    };

}