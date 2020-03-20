#pragma once

namespace SteelEngine { namespace Graphics {

    struct ITexture
    {
        virtual void Setup() = 0;

        virtual unsigned int GetTextureID() = 0;
        virtual unsigned int GetTextureWidth() = 0;
        virtual unsigned int GetTextureHeight() = 0;
    };

}}