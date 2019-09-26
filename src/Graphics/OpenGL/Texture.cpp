#include "Graphics/OpenGL/Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Graphics/OpenGL/stb_image.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    Texture::Texture(const std::string& filename)
    {
        m_Data = stbi_load(filename.c_str(), &m_Width, &m_Height, &m_Channels, 4);
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

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Data);

        stbi_image_free(m_Data);
    }

    void Texture::Cleanup()
    {
        glDeleteTextures(1, &m_Texture);
    }

    void Texture::Bind(Type::uint32 unit) const
    {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, m_Texture);
    }

}}}