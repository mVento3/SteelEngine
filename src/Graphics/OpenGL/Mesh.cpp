#include "Graphics/OpenGL/Mesh.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    Mesh::Mesh(const std::vector<Vertex>& vertices) :
        m_DrawCount(vertices.size()),
        m_Vertices(vertices)
    {
        m_HasTangents = false;
    }

    Mesh::Mesh(const std::string& filename)
    {
        m_HasTangents = false;

        Assimp::Importer importer;

        const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);

        if(scene)
        {
            aiMesh* mesh = scene->mMeshes[0];

            for(Type::uint32 i = 0; i < mesh->mNumVertices; i++)
            {
                glm::vec3 position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
                glm::vec3 normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
                glm::vec2 texCoord(0, 0);
                glm::vec3 tangent(0, 0, 0);

                if(mesh->mTangents)
                {
                    tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
                    m_HasTangents = true;
                }

                if(mesh->mTextureCoords[0])
                {
                    texCoord.x = mesh->mTextureCoords[0][i].x;
                    texCoord.y = mesh->mTextureCoords[0][i].y;
                }

                m_Vertices.push_back(Vertex(position, texCoord, normal, tangent));
            }

            for(Type::uint32 i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];

                for(Type::uint32 j = 0; j < face.mNumIndices; j++)
                {
                    m_Indicies.push_back(face.mIndices[j]);
                }
            }

            m_DrawCount = m_Indicies.size();
        }
    }

    Mesh::~Mesh()
    {

    }

    void Mesh::Setup()
    {
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> texCoords;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec3> tangents;

        for(Type::uint32 i = 0; i < m_Vertices.size(); i++)
        {
            vertices.push_back(m_Vertices[i].m_Position);
            texCoords.push_back(m_Vertices[i].m_TexCoord);
            normals.push_back(m_Vertices[i].m_Normal);
            tangents.push_back(m_Vertices[i].m_Tangent);
        }

        if(!m_HasTangents)
        {
            for(Type::uint32 i = 0; i < m_Indicies.size(); i += 3)
            {
                int i0 = m_Indicies[i];
                int i1 = m_Indicies[i + 1];
                int i2 = m_Indicies[i + 2];

                glm::vec3 edge1 = vertices[i1] - vertices[i0];
                glm::vec3 edge2 = vertices[i2] - vertices[i0];

                float deltaU1 = texCoords[i1].x - texCoords[i0].x;
                float deltaV1 = texCoords[i1].y - texCoords[i0].y;
                float deltaU2 = texCoords[i2].x - texCoords[i0].x;
                float deltaV2 = texCoords[i2].y - texCoords[i0].y;

                float dividend = deltaU1 * deltaV2 - deltaU2 * deltaV1;
                float f = dividend == 0 ? 0.f : 1.f / dividend;

                glm::vec3 tangent(
                    f * (deltaV2 * edge1.x - deltaV1 * edge2.x),
                    f * (deltaV2 * edge1.y - deltaV1 * edge2.y),
                    f * (deltaV2 * edge1.z - deltaV1 * edge2.z)
                );

                tangents[i0] = tangents[i0] + tangent;
                tangents[i1] = tangents[i0] + tangent;
                tangents[i2] = tangents[i0] + tangent;
            }

            for(Type::uint32 i = 0; i < m_Indicies.size(); i++)
            {
                tangents[i] = glm::normalize(tangents[i]);
            }
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

        glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[NORMAL_VB]);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(normals[0]), &normals[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[TANGENT_VB]);
        glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(tangents[0]), &tangents[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOs[INDEX_VB]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indicies.size() * sizeof(m_Indicies[0]), &m_Indicies[0], GL_STATIC_DRAW);

        glBindVertexArray(m_VAO);
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