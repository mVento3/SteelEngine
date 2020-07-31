#pragma once

#include "AssetManager/IAssetManager.h"

#include "stack"

#include "RuntimeReflection/Macro.h"

#include "AssetManager/Events/LoadAssetEvent.h"

namespace SteelEngine {

    SE_CLASS()
    class AssetManager : public IAssetManager
    {
    public:
        enum AssetType
        {
            MODEL_OBJ
        };

    private:
        std::vector<IAssetItem*> m_LoadedAssets;
        std::stack<IAssetItem::AssetID> m_AvailableIDs;

    public:
        AssetManager();
        ~AssetManager();

        IAssetItem* Load(const std::filesystem::path& path) override;
        IAssetItem* Load(IAssetItem::AssetID id) override;

        void operator()(LoadAssetEvent* event);
    };

}