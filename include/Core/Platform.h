#pragma once

#include "filesystem"

namespace SteelEngine {

#ifdef SE_WINDOWS
    namespace filesystem = std::experimental::filesystem;
#endif

}