#include "Graphics/OpenGL/Mesh.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    Mesh::Mesh()
    {
        m_VAO = std::numeric_limits<GLuint>().max();
        m_VBOs = 0;
    }

    Mesh::Mesh(const std::vector<Vertex>& vertices) :
        m_DrawCount(vertices.size())
    {
        m_VAO = std::numeric_limits<GLuint>().max();
        m_VBOs = 0;
    }

    Mesh::~Mesh()
    {

    }

    void Mesh::Begin(Type::uint32 countOfVBOs)
    {
        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);

        m_VBOs = new GLuint[countOfVBOs];

        glGenBuffers(countOfVBOs, m_VBOs);
    }

    void Mesh::PassVBO(Type::uint32 vboIndex, const void* data, Type::uint32 countOfData, size_t sizeOfDataElement, Type::uint32 countOfElements)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[vboIndex]);
        glBufferData(GL_ARRAY_BUFFER, countOfData * sizeOfDataElement, data, GL_STATIC_DRAW);
        glEnableVertexAttribArray(vboIndex);
        glVertexAttribPointer(vboIndex, countOfElements, GL_FLOAT, GL_FALSE, 0, 0);
    }

    void Mesh::PassVBO(Type::uint32 vboIndex, const void* data, Type::uint32 countOfData, size_t sizeOfDataElement)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOs[vboIndex]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, countOfData * sizeOfDataElement, data, GL_STATIC_DRAW);

        m_DrawCount = countOfData;
    }

    void Mesh::End()
    {
        glBindVertexArray(0);
    }

    void Mesh::Cleanup()
    {
        glDeleteVertexArrays(1, &m_VAO);
    }

    void Mesh::Draw() const
    {
        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES, m_DrawCount, GL_UNSIGNED_INT, 0);
    }

}}}