#include "Core/Result.h"

namespace SteelEngine { namespace Module {

    Result Load(const std::string& filename, void** dll);
    Result Get(const std::string& name, void* dll, void** result);
    Result Free(void** dll);
}}