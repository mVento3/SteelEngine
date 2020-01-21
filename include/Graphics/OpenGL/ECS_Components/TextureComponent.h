#pragma once

#include "Core/Type.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    struct TextureComponent
    {
        TextureComponent(Type::uint32 textureID) :
            m_TextureID(textureID)
        {

        }

        Type::uint32 m_TextureID;
    };

}}}