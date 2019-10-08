#include "Graphics/OpenGL/Framebuffer.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    Framebuffer::Framebuffer(Type::uint32 width, Type::uint32 height, const std::vector<Framebuffer::Attachment*>& attachments) :
        m_Attachments(attachments),
        m_Width(width),
        m_Height(height)
    {

    }

    Framebuffer::~Framebuffer()
    {

    }

    void Framebuffer::Setup()
    {
        std::vector<GLenum> drawBuffers;
        bool hasDepth = false;

        glGenFramebuffers(1, &m_FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

        if(m_FBO == 0)
        {
            // Print some error?

            return;
        }

        for(Framebuffer::Attachment* attachment : m_Attachments)
        {
            if(attachment->m_Attachment == GL_DEPTH_ATTACHMENT)
            {
                hasDepth = true;
                drawBuffers.push_back(GL_NONE);
            }
            else
            {
                drawBuffers.push_back(attachment->m_Attachment);
            }

            if(attachment->m_Texture->m_Width == 0 || attachment->m_Texture->m_Height)
            {
                attachment->m_Texture->m_Width = m_Width;
                attachment->m_Texture->m_Height = m_Height;
            }

            attachment->m_Texture->Setup();
            Bind();
            attachment->Setup();
        }

        if(!hasDepth)
        {
            glGenRenderbuffers(1, &m_RBO);
            glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Width, m_Height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RBO);
        }

        glDrawBuffers(drawBuffers.size(), drawBuffers.data());

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            printf("Problem with framebuffer: %i!\n", glGetError());
        }
    }

    void Framebuffer::Bind() const
    {
        glViewport(0, 0, m_Width, m_Height);
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    }

}}}