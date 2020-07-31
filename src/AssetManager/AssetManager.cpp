#include "AssetManager/AssetManager.h"

#include "AssetManager/Object3D_Asset.h"

namespace SteelEngine {

    AssetManager::AssetManager()
    {
        for(size_t i = 0; i < 10000; i++)
        {
            m_AvailableIDs.push(i);
        }

        Event::GlobalEvent::Add_<LoadAssetEvent>(this);
    }

    AssetManager::~AssetManager()
    {

    }

    IAssetItem* AssetManager::Load(const std::filesystem::path& path)
    {
    // Before we try to load an asset, lets check if it is not loaded first
        for(IAssetItem* asset : m_LoadedAssets)
        {
            if(asset->GetPath() == path)
            {
            // The asset is already loaded
                return asset;
            }
        }

        const std::string& ext = path.extension().string();
        IAssetItem* asset = 0;

    // It is propably 3D model, but it might be compile object too, need to keep eye on this...
    // TODO
        if(ext == ".obj")
        {
            asset = new Object3D_Asset(path);

            SetAssetID(asset, m_AvailableIDs.top());

            m_AvailableIDs.pop();
        }

    // After loading the asset might need some processing

        m_LoadedAssets.push_back(asset);

        return asset;
    }

    IAssetItem* AssetManager::Load(IAssetItem::AssetID id)
    {
        for(IAssetItem* asset : m_LoadedAssets)
        {
            if(asset->GetAssetID() == id)
            {
                return asset;
            }
        }

        return NULL;
    }

    void AssetManager::operator()(LoadAssetEvent* event)
    {
        event->m_Result = Load(event->m_Path);
    }

}