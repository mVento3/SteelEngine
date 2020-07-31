#pragma once

#include "AssetManager/IAssetItem.h"

#include "filesystem"

#include "Graphics/IRenderer.h"

namespace SteelEngine {

    struct IAssetManager
    {
    // TODO: In feature add maybe some expected types, cuz when some asset could not be loaded then we can return default type asset

        virtual IAssetItem* Load(const std::filesystem::path& path) = 0;
        virtual IAssetItem* Load(IAssetItem::AssetID id) = 0;

        void SetAssetID(IAssetItem* item, size_t id)
        {
            item->SetAssetID(id);
        }

        template <typename A>
        void SetAsset(AssetItem<A>* assetItem, A* asset)
        {
            assetItem->SetAsset(asset);
        }
    };

}