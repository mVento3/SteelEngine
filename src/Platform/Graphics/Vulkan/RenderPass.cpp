#include "Platform/Graphics/Vulkan/RenderPass.h"

namespace SteelEngine { namespace Platform { namespace Vulkan {

    RenderPass::RenderPass(LogicalDevice* lDevice) :
        m_LDevice(lDevice)
    {

    }

    RenderPass::~RenderPass()
    {

    }

    bool RenderPass::SetupRenderPass(const RenderPass::CreateInfo& createInfo)
    {
        VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = static_cast<uint32_t>(createInfo.m_Attachments.size());
		renderPassInfo.pAttachments = createInfo.m_Attachments.data();
		renderPassInfo.subpassCount = static_cast<uint32_t>(createInfo.m_Subpasses.size());
		renderPassInfo.pSubpasses = createInfo.m_Subpasses.data();
        renderPassInfo.dependencyCount = static_cast<uint32_t>(createInfo.m_Dependencies.size());;
		renderPassInfo.pDependencies = createInfo.m_Dependencies.data();

        if(!m_LDevice->CreateRenderPass(renderPassInfo, &m_RenderPass))
        {
            return false;
        }

        return true;
    }

}}}