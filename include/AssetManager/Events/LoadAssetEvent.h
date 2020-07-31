#pragma once

#include "AssetManager/IAssetItem.h"

namespace SteelEngine {

    struct LoadAssetEvent
    {
        std::filesystem::path m_Path;
        IAssetItem* m_Result;
    };

}