#pragma once

#include "string"
#include "vector"

#include "Graphics/OpenGL/Vertex.h"

#include "Core/Type.h"

#include "GL/glew.h"

#include "Graphics/IMesh.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    // Mesh should contains only raw data, not whole model loading stuff..

    class Mesh : public IMesh
    {
    private:
        GLuint m_VAO;
        GLuint* m_VBOs;
        Type::uint32 m_DrawCount;

    public:
        Mesh();
        Mesh(const std::vector<Vertex>& vertices);
        ~Mesh();

    // Start VAO
        void Begin(Type::uint32 countOfVBOs);

    // Pass to gpu some VBO
        void PassVBO(Type::uint32 vboIndex, const void* data, Type::uint32 countOfData, size_t sizeOfDataElement, Type::uint32 countOfElements);

        template <typename A>
        void PassVBO(Type::uint32 vboIndex, const std::vector<A>& data, Type::uint32 countOfElements)
        {
        // TODO: Some assert to prevent vector size be 0

            PassVBO(vboIndex, data.data(), data.size(), sizeof(data[0]), countOfElements);
        }

    // Pass to gpu indices
        void PassVBO(Type::uint32 vboIndex, const void* data, Type::uint32 countOfData, size_t sizeOfDataElement);

        template <typename A>
        void PassVBO(Type::uint32 vboIndex, const std::vector<A>& data)
        {
        // TODO: Some assert to prevent vector size be 0

            PassVBO(vboIndex, data.data(), data.size(), sizeof(data[0]));
        }

    // End VAO
        void End();

        void Cleanup();

        void Draw() const;

        inline GLuint GetVAO() const { return m_VAO; }
        inline GLuint GetDrawCount() const { return m_DrawCount; }
    };

}}}