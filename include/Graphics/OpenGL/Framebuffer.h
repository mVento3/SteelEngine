#pragma once

#include "GL/glew.h"

#include "Graphics/OpenGL/Texture.h"

#include "vector"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    class Framebuffer
    {
    public:
        struct Attachment
        {
            Attachment(Texture* texture, GLuint attachment) :
                m_Texture(texture),
                m_Attachment(attachment)
            {

            }

            Texture* m_Texture;
            GLuint m_Attachment;

            void Setup()
            {
                glFramebufferTexture2D(GL_FRAMEBUFFER, m_Attachment, GL_TEXTURE_2D, m_Texture->m_Texture, 0);
            }
        };

    private:
        GLuint m_FBO;

        std::vector<Attachment*> m_Attachments;

    public:
        Framebuffer(const std::vector<Attachment*>& attachments);
        ~Framebuffer();

        void Setup();

        void Bind() const;
        inline const GLuint& GetFBO() const { return m_FBO; }
    };

}}}