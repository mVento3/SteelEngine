#include "Rendering/Vulkan/Framebuffer.h"

#include "Rendering/Vulkan/Renderer.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    Framebuffer::Framebuffer()
    {

    }

    Framebuffer::~Framebuffer()
    {
        
    }

    Result Framebuffer::Create(Renderer* renderer)
    {
        m_SwapChainFramebuffers.resize(renderer->m_SwapChain->m_SwapChainImageViews.size());

        for(size_t i = 0; i < renderer->m_SwapChain->m_SwapChainImageViews.size(); i++)
        {
            VkImageView attachments[] =
            {
                renderer->m_SwapChain->m_SwapChainImageViews[i]
            };

            VkFramebufferCreateInfo framebufferInfo = {};

            framebufferInfo.sType =             VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass =        renderer->m_RenderPass->GetRenderPass();
            framebufferInfo.attachmentCount =   1;
            framebufferInfo.pAttachments =      attachments;
            framebufferInfo.width =             renderer->m_SwapChain->m_SwapChainExtent.width;
            framebufferInfo.height =            renderer->m_SwapChain->m_SwapChainExtent.height;
            framebufferInfo.layers =            1;

            if(vkCreateFramebuffer(renderer->m_LogicalDevice->GetLogicalDevice(), &framebufferInfo, nullptr, &m_SwapChainFramebuffers[i]) != VK_SUCCESS)
            {
                return SE_FALSE;
            }
        }

        return SE_TRUE;
    }

    void Framebuffer::Cleanup(Renderer* renderer)
    {
        for(auto framebuffer : m_SwapChainFramebuffers)
        {
            vkDestroyFramebuffer(renderer->m_LogicalDevice->GetLogicalDevice(), framebuffer, nullptr);
        }
    }

}}}