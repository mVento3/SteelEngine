#pragma once

#include "Core/Type.h"

#include "Math/Transform.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    struct RenderableComponent
    {
        RenderableComponent(Type::uint32 vao, Type::uint32 drawCount, Type::uint32 shaderID) :
            m_VAO(vao),
            m_DrawCount(drawCount),
            m_ShaderID(shaderID)
        {

        }

        Type::uint32 m_VAO;
        Type::uint32 m_DrawCount;
        Type::uint32 m_ShaderID;
    };

}}}