#include "Graphics/OpenGL/Mesh.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    Mesh::Mesh(const std::vector<Vertex>& vertices) :
        m_DrawCount(vertices.size()),
        m_Vertices(vertices)
    {

    }

    Mesh::~Mesh()
    {

    }

    void Mesh::Setup()
    {
        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);

        glGenBuffers(NUM_BUFFERS, m_VBOs);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[POSITION_VB]);
        glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(m_Vertices[0]), &m_Vertices[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindVertexArray(m_VAO);
    }

    void Mesh::Cleanup()
    {
        glDeleteVertexArrays(1, &m_VAO);
    }

    void Mesh::Draw() const
    {
        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, m_DrawCount);
    }

}}}