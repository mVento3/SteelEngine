#pragma once

#include "Core/Result.h"

namespace SteelEngine { namespace VirtualModule {

    Result memoryLoad(const std::string& filename, void** dll, void** memory);
    Result memoryGet(const std::string& name, void* dll, void** result);
    Result memoryFree(void** dll, void** memory);

}}