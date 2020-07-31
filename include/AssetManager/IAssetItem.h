#pragma once

#include "filesystem"

#undef TRUE
#undef FALSE

namespace SteelEngine {

    // TODO: Not sure about uint32_t GetAssetType ...

    struct IAssetItem
    {
        friend struct IAssetManager;
    public:
        typedef size_t AssetID;

        enum NeedProcess
        {
            FALSE = 0,
            TRUE = 1 << 0,
            PROCESSED = 1 << 1
        };

    protected:
        virtual void SetAssetID(AssetID id) = 0;

    public:
        virtual const std::filesystem::path& GetPath() const = 0;
        virtual uint32_t GetAssetType() const = 0;
        virtual AssetID GetAssetID() const = 0;
        virtual NeedProcess IsProcessNeeded() const = 0;
    };

    template <typename A>
    struct AssetItem : public IAssetItem
    {
        friend struct IAssetManager;
    protected:
        virtual void SetAssetID(AssetID id) override = 0;

        virtual void SetAsset(A* asset) = 0;

    public:
        virtual const std::filesystem::path& GetPath() const override = 0;
        virtual uint32_t GetAssetType() const override = 0;
        virtual AssetID GetAssetID() const override = 0;
        virtual NeedProcess IsProcessNeeded() const override = 0;

        virtual A* GetAsset() const = 0;
    };

}