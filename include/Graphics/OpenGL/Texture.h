#pragma once

#include "GL/glew.h"

#include "string"

#include "Core/Type.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    class Texture
    {
        friend struct Framebuffer;
    private:
        GLuint m_Texture;

        unsigned char* m_Data;
        int m_Width;
        int m_Height;
        int m_Channels;

        GLint m_InternalFormat;
        GLenum m_Format;
        GLenum m_Type;
        GLfloat m_MinFilter;
        GLfloat m_MagFilter;

    public:
        Texture(const std::string& filename);
        Texture(int width, int height, GLint internalFormat, GLenum format, GLenum type, GLfloat minFilter, GLfloat magFilter);
        ~Texture();

        void Setup();
        void Cleanup();

        void Bind(GLint texture) const;
    };

}}}