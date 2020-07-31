#include "AssetManager/Object3D_Asset.h"

namespace SteelEngine {

    Object3D_Asset::Object3D_Asset(const std::filesystem::path& path) :
        m_Path(path)
    {
        m_AssetType = AssetManager::AssetType::MODEL_OBJ;
        m_Process = IAssetItem::NeedProcess::TRUE;

        SetAsset(Graphics::Model::Create(path.string().c_str()));
    }

    Object3D_Asset::~Object3D_Asset()
    {

    }

}