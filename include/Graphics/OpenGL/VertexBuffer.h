#pragma once

#include "Graphics/VertexBuffer.h"

#include "GL/glew.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    class VertexBuffer : public Graphics::VertexBuffer
    {
    private:
        GLuint m_ID;

    public:
        VertexBuffer(float* vertices, Type::uint32 count);
        ~VertexBuffer();

        void Bind() const override;
        void Unbind() const override;

        Type::uint32 GetID() const override { return m_ID; }
    };

}}}