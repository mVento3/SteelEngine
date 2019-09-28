#include "Graphics/OpenGL/Mesh.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    Mesh::Mesh(const std::vector<Vertex>& vertices) :
        m_DrawCount(vertices.size()),
        m_Vertices(vertices)
    {

    }

    Mesh::Mesh(const std::string& filename)
    {
        Assimp::Importer importer;

        const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);

        if(scene)
        {
            aiMesh* mesh = scene->mMeshes[0];

            for(Type::uint32 i = 0; i < mesh->mNumVertices; i++)
            {
                glm::vec3 position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
                glm::vec2 texCoord;

                if(mesh->mTextureCoords[0])
                {
                    texCoord.x = mesh->mTextureCoords[0][i].x;
                    texCoord.y = mesh->mTextureCoords[0][i].y;
                }

                m_Vertices.push_back(Vertex(position, texCoord));
            }

            m_DrawCount = m_Vertices.size();
        }
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