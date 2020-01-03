#pragma once

#include "Core/Type.h"

#include "filesystem"

namespace SteelEngine {

    struct LogData
    {
        Type::uint32 m_Line;
        std::filesystem::path m_File;
        std::string m_Message;
        int m_Verbosity;
    };

}