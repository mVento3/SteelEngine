#include "Platform/Graphics/Vulkan/Framebuffer.h"

#include "Platform/Graphics/Vulkan/RenderPass.h"

namespace SteelEngine { namespace Platform { namespace Vulkan {

    Framebuffer::Framebuffer(RenderPass* renderPass, const VkExtent2D& renderArea) :
        m_RenderPass(renderPass),
        m_RenderArea(renderArea)
    {

    }

    Framebuffer::~Framebuffer()
    {

    }

    bool Framebuffer::SetupFramebuffer(const LogicalDevice* lDevice, const CreateInfo& createInfo)
    {
        VkFramebufferCreateInfo fbufCreateInfo = {};
        fbufCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		fbufCreateInfo.renderPass = m_RenderPass->m_RenderPass;
		fbufCreateInfo.attachmentCount = static_cast<uint32_t>(createInfo.m_Attachments.size());
		fbufCreateInfo.pAttachments = createInfo.m_Attachments.data();
		fbufCreateInfo.width = m_RenderArea.width;
		fbufCreateInfo.height = m_RenderArea.height;
		fbufCreateInfo.layers = 1;

        if(!lDevice->CreateFramebuffer(fbufCreateInfo, &m_Framebuffer))
        {
            return false;
        }

        return true;
    }

}}}