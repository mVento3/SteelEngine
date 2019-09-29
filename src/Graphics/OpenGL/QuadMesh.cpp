#include "Graphics/OpenGL/QuadMesh.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    QuadMesh::QuadMesh()
    {
        m_DrawCount = 4;

        m_Vertices =
        {
            Vertex(glm::vec3(-1, 1, 0), glm::vec2(0, 1), glm::vec3(), glm::vec3()),
            Vertex(glm::vec3(-1, -1, 0), glm::vec2(0, 0), glm::vec3(), glm::vec3()),
            Vertex(glm::vec3(1, 1, 0), glm::vec2(1, 1), glm::vec3(), glm::vec3()),
            Vertex(glm::vec3(1, -1, 0), glm::vec2(1, 0), glm::vec3(), glm::vec3())
        };
    }

    QuadMesh::~QuadMesh()
    {

    }

    void QuadMesh::Setup()
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

    void QuadMesh::Cleanup()
    {
        glDeleteVertexArrays(1, &m_VAO);
    }

    void QuadMesh::Draw() const
    {
        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, m_DrawCount);
    }

}}}