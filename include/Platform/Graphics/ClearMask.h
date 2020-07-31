#pragma once

namespace SteelEngine {

    enum class ClearMask
    {
        COLOR_BUFFER_BIT = 1 << 0,
        DEPTH_BUFFER_BIT = 1 << 1
    };

    inline ClearMask operator|(ClearMask a, ClearMask b)
    {
        return static_cast<ClearMask>(static_cast<int>(a) | static_cast<int>(b));
    }

    inline bool operator&(ClearMask a, ClearMask b)
    {
        return static_cast<int>(a) & static_cast<int>(b);
    }

}