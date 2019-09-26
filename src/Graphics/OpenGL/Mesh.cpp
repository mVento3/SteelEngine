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
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> texCoords;

        for(Type::uint32 i = 0; i < m_Vertices.size(); i++)
        {
            vertices.push_back(m_Vertices[i].m_Position);
            texCoords.push_back(m_Vertices[i].m_TexCoord);
        }

        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);

        glGenBuffers(NUM_BUFFERS, m_VBOs);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[POSITION_VB]);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[TEXTURE_VB]);
        glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(texCoords[0]), &texCoords[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

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