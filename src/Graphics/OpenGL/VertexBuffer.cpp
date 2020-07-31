#include "Graphics/OpenGL/VertexBuffer.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    VertexBuffer::VertexBuffer(float* vertices, Type::uint32 count)
    {
        glGenBuffers(1, &m_ID);

        glBindBuffer(GL_ARRAY_BUFFER, m_ID);
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), vertices, GL_STATIC_DRAW);
    }

    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers(1, &m_ID);
    }

    void VertexBuffer::Bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    }

    void VertexBuffer::Unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

}}}