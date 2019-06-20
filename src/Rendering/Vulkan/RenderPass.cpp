#include "Rendering/Vulkan/RenderPass.h"

#include "Rendering/Vulkan/Renderer.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    RenderPass::RenderPass()
    {

    }

    RenderPass::~RenderPass()
    {

    }

    Result RenderPass::Create(Renderer* renderer)
    {
        VkAttachmentDescription colorAttachment = {};

        colorAttachment.format =            renderer->m_SwapChain->m_SwapChainImageFormat;
        colorAttachment.samples =           VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp =            VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp =           VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp =     VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp =    VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout =     VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout =       VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef = {};

        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout =     VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass = {};

        subpass.pipelineBindPoint =     VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount =  1;
        subpass.pColorAttachments =     &colorAttachmentRef;

        VkRenderPassCreateInfo renderPassInfo = {};

        renderPassInfo.sType =              VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount =    1;
        renderPassInfo.pAttachments =       &colorAttachment;
        renderPassInfo.subpassCount =       1;
        renderPassInfo.pSubpasses =         &subpass;

        VkSubpassDependency dependency = {};

        dependency.srcSubpass =     VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass =     0;
        dependency.srcStageMask =   VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask =  0;
        dependency.dstStageMask =   VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask =  VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        renderPassInfo.dependencyCount =    1;
        renderPassInfo.pDependencies =      &dependency;

        if(vkCreateRenderPass(renderer->m_LogicalDevice->GetLogicalDevice(), &renderPassInfo, nullptr, &m_RenderPass) != VK_SUCCESS)
        {
            return SE_FALSE;
        }

        return SE_TRUE;
    }

}}}