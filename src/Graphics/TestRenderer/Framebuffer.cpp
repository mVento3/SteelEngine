#include "Graphics/TestRenderer/Framebuffer.h"

namespace SteelEngine { namespace Graphics { namespace TestRenderer {

    Framebuffer::Framebuffer(uint32_t width, uint32_t height, std::vector<FramebufferAttachment> attachments) :
        m_Width(width),
        m_Height(height),
        m_Attachments(attachments)
    {
        m_FBO = 0;
        m_RBO = 0;
    }

    Framebuffer::~Framebuffer()
    {

    }

    void Framebuffer::Setup(const RenderDevice* renderDevice)
    {
        bool hasDepth = false;
        std::vector<uint32_t> drawBuffers;

        renderDevice->GenerateFramebuffer(this);
        renderDevice->Bind(this);

        if(m_FBO == 0)
        {
            return;
        }

        for(FramebufferAttachment attachment : m_Attachments)
        {
            renderDevice->FramebufferTexture2D(this, attachment, &hasDepth, drawBuffers);
        }

        if(!hasDepth)
        {
            renderDevice->GenRenderbuffer(this);
            renderDevice->BindRenderbuffer(this);
            renderDevice->RenderbufferStorage(this);
            renderDevice->FramebufferRenderbuffer(this);
        }

        renderDevice->DrawBuffers(drawBuffers);

        if(renderDevice->CheckFramebufferStatus() != FramebufferStatus::FRAMEBUFFER_COMPLETE)
        {
            printf("Problem with framebuffer: %i!\n", glGetError());
        }
    }

    void Framebuffer::Bind(const RenderDevice* renderDevice) const
    {
        renderDevice->Viewport(this);
        renderDevice->Bind(this);
    }

    // void Framebuffer::Setup()
    // {
    //     std::vector<GLenum> drawBuffers;
    //     bool hasDepth = false;

    //     glGenFramebuffers(1, &m_FBO);
    //     glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

    //     if(m_FBO == 0)
    //     {
    //         // Print some error?

    //         return;
    //     }

    //     for(FramebufferAttachment* attachment : m_Attachments)
    //     {
    //         if(attachment->m_Attachment == FramebufferAttachment::Attachment::DEPTH_ATTACHMENT)
    //         {
    //             hasDepth = true;
    //             drawBuffers.push_back(GL_NONE);
    //         }
    //         else
    //         {
    //             //attach = attachment->m_Attachment;//GL_COLOR_ATTACHMENT0 + attachment->m_Attachment;

    //             drawBuffers.push_back(attachment->m_Attachment);
    //         }

    //         if(attachment->m_Texture->GetTextureWidth() == 0 || attachment->m_Texture->GetTextureHeight())
    //         {
    //             attachment->m_Texture->SetWidth(m_Width);
    //             attachment->m_Texture->SetHeight(m_Height);
    //         }

    //         attachment->m_Texture->Setup();
    //         glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    //         glFramebufferTexture2D(GL_FRAMEBUFFER, attachment->m_Attachment, GL_TEXTURE_2D, attachment->m_Texture->GetTextureID(), 0);
    //     }

    //     if(!hasDepth)
    //     {
    //         glGenRenderbuffers(1, &m_RBO);
    //         glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
    //         glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Width, m_Height);
    //         glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RBO);
    //     }

    //     glDrawBuffers(drawBuffers.size(), drawBuffers.data());

    //     if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    //     {
    //         printf("Problem with framebuffer: %i!\n", glGetError());
    //     }
    // }

    // void Framebuffer::Bind() const
    // {
    //     glViewport(0, 0, m_Width, m_Height);
    //     glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    // }

}}}