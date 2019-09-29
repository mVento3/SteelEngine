#pragma once

#include "string"
#include "vector"

#include "Graphics/OpenGL/Vertex.h"

#include "Core/Type.h"

#include "GL/glew.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    class QuadMesh
    {
    public:
        enum Buffer
        {
            POSITION_VB,
            TEXTURE_VB,

            NUM_BUFFERS
        };

    private:
        GLuint m_VAO;
        GLuint m_VBOs[NUM_BUFFERS];
        Type::uint32 m_DrawCount;

        std::vector<Vertex> m_Vertices;

    public:
        QuadMesh();
        ~QuadMesh();

        void Setup();
        void Cleanup();

        void Draw() const;
    };

}}}