#include "Graphics/OpenGL/IndexBuffer.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    IndexBuffer::IndexBuffer(Type::uint32* indices, Type::uint32 count) :
        m_Count(count)
    {
        glGenBuffers(1, &m_ID);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(Type::uint32), indices, GL_STATIC_DRAW);
    }

    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers(1, &m_ID);
    }

    void IndexBuffer::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
    }

    void IndexBuffer::Unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

}}}