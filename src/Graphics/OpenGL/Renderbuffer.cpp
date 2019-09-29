#include "Graphics/OpenGL/Renderbuffer.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    Renderbuffer::Renderbuffer()
    {

    }

    Renderbuffer::~Renderbuffer()
    {

    }

    void Renderbuffer::Setup()
    {
        glGenRenderbuffers(1, &m_RBO);
        glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);

        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1600, 900);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RBO);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            printf("OpenGL error: Framebuffer not complete!\n");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

}}}