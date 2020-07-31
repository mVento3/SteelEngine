#pragma once

#include "string"
#include "vector"

#include "Graphics/IModel.h"

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/Reflection.h"

#include "glm/glm.hpp"

#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

namespace SteelEngine { namespace Graphics {

    SE_CLASS()
    class Model : public IModel
    {
    public:
        enum Buffer
        {
            POSITION_VB,
            TEXTURE_VB,
            NORMAL_VB,
            TANGENT_VB,
            INDEX_VB,

            NUM_BUFFERS
        };

    private:
        std::string m_Filename;

        std::vector<glm::vec3> m_Positions;
        std::vector<glm::vec2> m_TexCoords;
        std::vector<glm::vec3> m_Normals;
        std::vector<glm::vec3> m_Tangents;
        std::vector<unsigned int> m_Indices;

        Type::uint32 m_DrawCount;

        std::vector<glm::vec3> CalculateNormals(const std::vector<glm::vec3>& vertices);
        std::vector<glm::vec3> CalculateTangents(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec2>& texCoords);

    public:
        Model(const char* file);
        Model(std::vector<glm::vec3> vertices, std::vector<glm::vec2> texCoords, std::vector<Type::uint32> indices);
        ~Model();

        static IModel* Create(const char* file)
        {
            IModel* model = (IModel*)Reflection::CreateInstance<Model>(file);

            return model;
        }

        static IModel* Create(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec2>& texCoords, const std::vector<Type::uint32>& indices)
        {
            IModel* model = (IModel*)Reflection::CreateInstance<Model, const std::vector<glm::vec3>&, const std::vector<glm::vec2>&, const std::vector<glm::uint32>&>(vertices, texCoords, indices);

            return model;
        }

        IMesh* Setup() override;

        const std::vector<glm::vec3>& GetVertices() const { return m_Positions; }
        const std::vector<glm::vec2>& GetTexCoords() const { return m_TexCoords; }
        const std::vector<glm::vec3>& GetNormals() const { return m_Normals; }
        const std::vector<glm::vec3>& GetTangents() const { return m_Tangents; }
        const std::vector<uint32_t>& GetIndices() const { return m_Indices; }
    };

}}