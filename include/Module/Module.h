#pragma once

#include "Core/Result.h"

#include "filesystem"

namespace SteelEngine { namespace Module {

    Result load(const char* filename, void** dll);
    // Result load(const std::string& filename, void** dll);
    // Result load(const std::filesystem::path& filename, void** dll);
    Result get(const std::string& name, void* dll, void** result);
    Result free(void** dll);

}}