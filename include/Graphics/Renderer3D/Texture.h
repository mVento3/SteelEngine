#pragma once

#include "GL/glew.h"

#include "Graphics/ITexture.h"

#include "string"

#include "Core/Type.h"

namespace SteelEngine { namespace Graphics { namespace Renderer3D {

    class Texture : public ITexture
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
        GLfloat m_Filter;
        GLenum m_TextureTarget;
        bool m_Clamp;

    public:
        Texture(const std::string& filename);
        Texture(int width, int height, GLint internalFormat, GLenum format, GLenum type, GLfloat filter, bool clamp);
        Texture(GLint internalFormat, GLenum format, GLenum type, GLfloat filter, bool clamp);
        ~Texture();

        void Setup() override;
        void Cleanup();

        void Bind(GLint texture) const;

        unsigned int GetTextureID() const override { return m_Texture; }
        unsigned int GetTextureWidth() const override { return m_Width; }
        unsigned int GetTextureHeight() const override { return m_Height; }
    };

}}}