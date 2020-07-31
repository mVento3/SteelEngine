#pragma once

#include "Graphics/IndexBuffer.h"

#include "GL/glew.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    class IndexBuffer : public Graphics::IndexBuffer
    {
    private:
        Type::uint32 m_Count;

        GLuint m_ID;

    public:
        IndexBuffer(Type::uint32* indices, Type::uint32 count);
        ~IndexBuffer();

        void Bind() const override;
        void Unbind() const override;

        Type::uint32 GetCount() const override { return m_Count; }
        Type::uint32 GetID() const override { return m_ID; }
    };

}}}