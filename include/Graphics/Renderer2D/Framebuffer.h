#pragma once

#include "GL/glew.h"

#include "Graphics/Renderer2D/Texture.h"

#include "vector"

namespace SteelEngine { namespace Graphics { namespace Renderer2D {

    class Framebuffer
    {
    public:
        struct Attachment
        {
            Attachment(Texture* texture, GLenum attachment) :
                m_Texture(texture),
                m_Attachment(attachment)
            {

            }

            Texture* m_Texture;
            GLenum m_Attachment;

            void Setup()
            {
                glFramebufferTexture2D(GL_FRAMEBUFFER, m_Attachment, GL_TEXTURE_2D, m_Texture->m_Texture, 0);
            }
        };

    private:
        GLuint m_FBO;
        GLuint m_RBO;

        Type::uint32 m_Width;
        Type::uint32 m_Height;

        std::vector<Attachment*> m_Attachments;

    public:
        Framebuffer(Type::uint32 width, Type::uint32 height, const std::vector<Attachment*>& attachments);
        ~Framebuffer();

        void Setup();

        void Bind() const;
        inline const GLuint& GetFBO() const { return m_FBO; }
    };

}}}