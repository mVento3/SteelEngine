#include "Graphics/OpenGL/Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Graphics/OpenGL/stb_image.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    Texture::Texture(const std::string& filename)
    {
        m_Data = stbi_load(filename.c_str(), &m_Width, &m_Height, &m_Channels, 4);

        m_MinFilter = GL_LINEAR;
        m_MagFilter = GL_LINEAR;
        m_InternalFormat = GL_RGBA;
        m_Format = GL_RGBA;
        m_Type = GL_UNSIGNED_BYTE;
    }

    Texture::Texture(int width, int height, GLint internalFormat, GLenum format, GLenum type, GLfloat minFilter, GLfloat magFilter) :
        m_Width(width),
        m_Height(height),
        m_InternalFormat(internalFormat),
        m_Format(format),
        m_Type(type),
        m_MinFilter(minFilter),
        m_MagFilter(magFilter)
    {
        m_Data = 0;
    }

    Texture::~Texture()
    {

    }

    void Texture::Setup()
    {
        glGenTextures(1, &m_Texture);
        glBindTexture(GL_TEXTURE_2D, m_Texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_MinFilter);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_MagFilter);

        glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_Format, m_Type, m_Data);

        if(m_Data)
        {
            stbi_image_free(m_Data);
        }
    }

    void Texture::Cleanup()
    {
        glDeleteTextures(1, &m_Texture);
    }

    void Texture::Bind(GLint texture) const
    {
        glActiveTexture(texture);
        glBindTexture(GL_TEXTURE_2D, m_Texture);
    }

}}}