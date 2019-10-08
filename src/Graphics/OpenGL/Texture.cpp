#include "Graphics/OpenGL/Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Graphics/OpenGL/stb_image.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    Texture::Texture(const std::string& filename)
    {
        m_Data = stbi_load(filename.c_str(), &m_Width, &m_Height, &m_Channels, 4);

        m_Filter = GL_LINEAR;
        m_InternalFormat = GL_RGBA;
        m_Format = GL_RGBA;
        m_Type = GL_UNSIGNED_BYTE;
        m_TextureTarget = GL_TEXTURE_2D;
        m_Clamp = false;
    }

    Texture::Texture(int width, int height, GLint internalFormat, GLenum format, GLenum type, GLfloat filter, bool clamp) :
        m_Width(width),
        m_Height(height),
        m_InternalFormat(internalFormat),
        m_Format(format),
        m_Type(type),
        m_Filter(filter),
        m_Clamp(clamp)
    {
        m_Data = 0;

        m_TextureTarget = GL_TEXTURE_2D;
    }

    Texture::Texture(GLint internalFormat, GLenum format, GLenum type, GLfloat filter, bool clamp) :
        m_InternalFormat(internalFormat),
        m_Format(format),
        m_Type(type),
        m_Filter(filter),
        m_Clamp(clamp)
    {
        m_Data = 0;

        m_TextureTarget = GL_TEXTURE_2D;

        m_Width = 0;
        m_Height = 0;
    }

    Texture::~Texture()
    {

    }

    void Texture::Setup()
    {
        glGenTextures(1, &m_Texture);
        glBindTexture(m_TextureTarget, m_Texture);

        if(!m_Clamp)
        {
            glTexParameterf(m_TextureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameterf(m_TextureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }
        else
        {
            glTexParameterf(m_TextureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(m_TextureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        glTexParameterf(m_TextureTarget, GL_TEXTURE_MIN_FILTER, m_Filter);
        glTexParameterf(m_TextureTarget, GL_TEXTURE_MAG_FILTER, m_Filter);

        glTexImage2D(m_TextureTarget, 0, m_InternalFormat, m_Width, m_Height, 0, m_Format, m_Type, m_Data);

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
        glBindTexture(m_TextureTarget, m_Texture);
    }

}}}