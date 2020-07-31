#include "Graphics/Model.h"

#include "Graphics/IRenderer.h"

#include "Graphics/OpenGL/Mesh.h"

#include "Logger/Logger.h"

#include "Core/Core.h"

#include "Utils/Graphics/RenderContext.h"

namespace SteelEngine { namespace Graphics {

    std::vector<glm::vec3> Model::CalculateNormals(const std::vector<glm::vec3>& vertices)
    {
        std::vector<glm::vec3> normals;

        for(Type::uint32 i = 0; i < vertices.size(); i += 3)
        {
            glm::vec3 v1 = vertices[i + 2] - vertices[i + 1];
            glm::vec3 v2 = vertices[i] - vertices[i + 1];
            glm::vec3 v3 = glm::cross(v1, v2);

            normals.push_back(glm::normalize(v3));
            normals.push_back(glm::normalize(v3));
            normals.push_back(glm::normalize(v3));
        }

        return normals;
    }

    std::vector<glm::vec3> Model::CalculateTangents(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec2>& texCoords)
    {
        std::vector<glm::vec3> tangents;

        for(Type::uint32 i = 0; i < vertices.size(); i += 3)
        {
            const glm::vec3& v0 = vertices[i + 0];
            const glm::vec3& v1 = vertices[i + 1];
            const glm::vec3& v2 = vertices[i + 2];

            const glm::vec2& uv0 = texCoords[i + 0];
            const glm::vec2& uv1 = texCoords[i + 1];
            const glm::vec2& uv2 = texCoords[i + 2];

            glm::vec3 deltaPos1 = v1 - v0;
            glm::vec3 deltaPos2 = v2 - v0;

            glm::vec2 deltaUV1 = uv1 - uv0;
            glm::vec2 deltaUV2 = uv2 - uv0;

            float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
            glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
            // glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

            tangents.push_back(tangent);
            tangents.push_back(tangent);
            tangents.push_back(tangent);
        }

        return tangents;
    }

    Model::Model(const char* file) :
        m_Filename(file)
    {
        Assimp::Importer importer;

        const aiScene* scene = importer.ReadFile(file, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

        if(scene)
        {
            aiMesh* mesh = scene->mMeshes[0];
            const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);

            for(Type::uint32 i = 0; i < mesh->mNumVertices; i++)
            {
                const aiVector3D pos = mesh->mVertices[i];
                const aiVector3D normal = mesh->mNormals[i];
                const aiVector3D texCoord = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][i] : aiZeroVector;
                const aiVector3D tangent = mesh->mTangents[i];

                m_Positions.push_back(glm::vec3(pos.x, pos.y, pos.z));
                m_TexCoords.push_back(glm::vec2(texCoord.x, texCoord.y));
                m_Normals.push_back(glm::vec3(normal.x, normal.y, normal.z));
                m_Tangents.push_back(glm::vec3(tangent.x, tangent.y, tangent.z));
            }

            for(unsigned int i = 0; i < mesh->mNumFaces; i++)
            {
                const aiFace& face = mesh->mFaces[i];

                assert(face.mNumIndices == 3);

                m_Indices.push_back(face.mIndices[0]);
                m_Indices.push_back(face.mIndices[1]);
                m_Indices.push_back(face.mIndices[2]);
            }

            if(!mesh->HasNormals())
            {
                m_Normals = CalculateNormals(m_Positions);
            }

            if(!mesh->HasTangentsAndBitangents())
            {
                m_Tangents = CalculateTangents(m_Positions, m_TexCoords);
            }

            m_DrawCount = m_Indices.size();
        }
    }

    Model::Model(std::vector<glm::vec3> vertices, std::vector<glm::vec2> texCoords, std::vector<Type::uint32> indices) :
        m_Positions(vertices),
        m_TexCoords(texCoords),
        m_Indices(indices)
    {
        m_Normals = CalculateNormals(m_Positions);
        m_Tangents = CalculateTangents(m_Positions, m_TexCoords);
    }

    Model::~Model()
    {

    }

    IMesh* Model::Setup()
    {
        switch(SteelEngine::Utils::RenderContext::GetCurrentAPI())
        {
        case IRenderer::API::OPENGL:
            {
                OpenGL::Mesh* mesh = new OpenGL::Mesh();

                mesh->Begin(NUM_BUFFERS);
                {
                    mesh->PassVBO(POSITION_VB, m_Positions, 3);
                    mesh->PassVBO(TEXTURE_VB, m_TexCoords, 2);
                    mesh->PassVBO(NORMAL_VB, m_Normals, 3);
                    mesh->PassVBO(TANGENT_VB, m_Tangents, 3);
                    mesh->PassVBO(INDEX_VB, m_Indices);
                }
                mesh->End();

                return mesh;
            }
        case IRenderer::API::VULKAN:
            SE_INFO("Vulkan api is not available yet!");
            break;
        default:
            SE_WARNING("Requested graphics api is not available!");
            break;
        }
    }

}}