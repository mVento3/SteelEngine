#pragma once

#include "GL/glew.h"

#include "string"

#include "Core/Type.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    class Texture
    {
    private:
        GLuint m_Texture;

        unsigned char* m_Data;
        int m_Width;
        int m_Height;
        int m_Channels;

    public:
        Texture(const std::string& filename);
        ~Texture();

        void Setup();
        void Cleanup();

        void Bind(Type::uint32 unit) const;
    };

}}}