#pragma once

#include "Core/Result.h"

namespace SteelEngine { namespace Module {

    Result load(const std::string& filename, void** dll);
    Result get(const std::string& name, void* dll, void** result);
    Result free(void** dll);

}}