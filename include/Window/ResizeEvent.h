#pragma once

#include "Core/Type.h"

namespace SteelEngine {

    struct ResizeEvent
    {
        Type::uint32 m_Width;
        Type::uint32 m_Height;
    };

}