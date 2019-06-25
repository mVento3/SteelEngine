#include "Rendering/Vulkan/Framebuffer.h"

#include "Rendering/Vulkan/SwapChain.h"
#include "Rendering/Vulkan/LogicalDevice.h"
#include "Rendering/Vulkan/RenderPass.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    Framebuffer::Framebuffer()
    {

    }

    Framebuffer::~Framebuffer()
    {
        
    }

    Result Framebuffer::Create(const LogicalDevice& logicalDevice, const SwapChain& swapChain, const RenderPass& renderPass)
    {
        m_SwapChainFramebuffers.resize(swapChain.m_SwapChainImageViews.size());

        for(size_t i = 0; i < swapChain.m_SwapChainImageViews.size(); i++)
        {
            VkImageView attachments[] =
            {
                swapChain.m_SwapChainImageViews[i]
            };

            VkFramebufferCreateInfo framebufferInfo = {};

            framebufferInfo.sType =             VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass =        renderPass.GetRenderPass();
            framebufferInfo.attachmentCount =   1;
            framebufferInfo.pAttachments =      attachments;
            framebufferInfo.width =             swapChain.m_SwapChainExtent.width;
            framebufferInfo.height =            swapChain.m_SwapChainExtent.height;
            framebufferInfo.layers =            1;

            if(vkCreateFramebuffer(logicalDevice.GetLogicalDevice(), &framebufferInfo, nullptr, &m_SwapChainFramebuffers[i]) != VK_SUCCESS)
            {
                return SE_FALSE;
            }
        }

        return SE_TRUE;
    }

    void Framebuffer::Cleanup(const LogicalDevice& logicalDevice)
    {
        for(auto framebuffer : m_SwapChainFramebuffers)
        {
            vkDestroyFramebuffer(logicalDevice.GetLogicalDevice(), framebuffer, nullptr);
        }
    }

}}}