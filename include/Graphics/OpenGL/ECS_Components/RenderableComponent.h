#pragma once

#include "Core/Type.h"

#include "Math/Transform.h"

#include "Graphics/OpenGL/Mesh.h"
#include "Graphics/OpenGL/Shader.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    struct RenderableComponent
    {
        RenderableComponent(const Mesh* mesh, const Shader* shader)
        {
            m_VAO = mesh->GetVAO();
            m_DrawCount = mesh->GetDrawCount();
            m_ShaderID = shader->GetShaderID();
        }

        Type::uint32 m_VAO;
        Type::uint32 m_DrawCount;
        Type::uint32 m_ShaderID;
    };

}}}