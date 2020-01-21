#pragma once

#include "string"
#include "vector"

#include "Graphics/OpenGL/Vertex.h"

#include "Core/Type.h"

#include "GL/glew.h"

#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

#include "Graphics/IMesh.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    class Mesh : public IMesh
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
        GLuint m_VAO;
        GLuint m_VBOs[NUM_BUFFERS];
        Type::uint32 m_DrawCount;
        bool m_HasTangents;

        std::vector<glm::vec3> m_Positions;
        std::vector<glm::vec2> m_TexCoords;
        std::vector<glm::vec3> m_Normals;
        std::vector<glm::vec3> m_Tangents;
        std::vector<unsigned int> m_Indicies;

    public:
        Mesh(const std::vector<Vertex>& vertices);
        Mesh(const std::string& filename);
        ~Mesh();

        void Setup();
        void Cleanup();

        void Draw() const;

        inline GLuint GetVAO() const { return m_VAO; }
        inline GLuint GetDrawCount() const { return m_DrawCount; }
    };

}}}