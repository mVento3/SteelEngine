#pragma once

#include "Core/Type.h"

#include "Math/Transform.h"

#include "Graphics/OpenGL/Mesh.h"
#include "Graphics/OpenGL/Shader.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    struct RenderableComponent
    {
        RenderableComponent(const Mesh* mesh, const Shader* shader, const Transform& transform = Transform(), bool castShadow = true) :
            m_CastShadow(castShadow),
            m_Transform(transform)
        {
            m_VAO = mesh->GetVAO();
            m_DrawCount = mesh->GetDrawCount();
            m_ShaderID = shader->GetShaderID();
        }

        Type::uint32 m_VAO;
        Type::uint32 m_DrawCount;
        Type::uint32 m_ShaderID;
        bool m_CastShadow;
        Transform m_Transform;
    };

}}}