#include "Graphics/OpenGL/Framebuffer.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    Framebuffer::Framebuffer(const std::vector<Framebuffer::Attachment*>& attachments) :
        m_Attachments(attachments)
    {

    }

    Framebuffer::~Framebuffer()
    {

    }

    void Framebuffer::Setup()
    {
        glGenFramebuffers(1, &m_FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

        for(Framebuffer::Attachment* attachment : m_Attachments)
        {
            attachment->Setup();
        }
    }

    void Framebuffer::Bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    }

}}}