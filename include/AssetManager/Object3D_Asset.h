#pragma once

#include "AssetManager/IAssetItem.h"
#include "AssetManager/AssetManager.h"

#include "Graphics/Model.h"

#include "RuntimeReflection/Macro.h"

namespace SteelEngine {

    SE_CLASS()
    class Object3D_Asset : public AssetItem<Graphics::IModel>
    {
    private:
        AssetManager::AssetType m_AssetType;
        std::filesystem::path m_Path;
        AssetID m_ID;
        NeedProcess m_Process;
        Graphics::IModel* m_Asset;

        void SetAssetID(AssetID id) override { m_ID = id; }

        void SetAsset(Graphics::IModel* model) override { m_Asset = model; }

    public:
        Object3D_Asset(const std::filesystem::path& path);
        ~Object3D_Asset();

        const std::filesystem::path& GetPath() const override { return m_Path; }
        uint32_t GetAssetType() const override { return m_AssetType; }
        AssetID GetAssetID() const override { return m_ID; }
        NeedProcess IsProcessNeeded() const override { return m_Process; }

        Graphics::IModel* GetAsset() const override { return m_Asset; }
    };

}