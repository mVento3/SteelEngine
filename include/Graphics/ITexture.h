#pragma once

namespace SteelEngine { namespace Graphics {

    struct ITexture
    {
        virtual unsigned int GetTextureID() = 0;
        virtual void Setup() = 0;
    };

}}