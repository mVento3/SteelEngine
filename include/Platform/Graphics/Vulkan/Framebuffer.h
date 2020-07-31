#pragma once

#include "Platform/Graphics/Vulkan/LogicalDevice.h"
#include "Platform/Graphics/Vulkan/Texture.h"

namespace SteelEngine { namespace Platform { namespace Vulkan {

    class RenderPass;
    class CommandBuffer;

    class Framebuffer
    {
        friend class CommandBuffer;
    public:
        struct CreateInfo
        {
            std::vector<VkImageView> m_Attachments;

            void AddAttchamnet(const VkImageView& imageView)
            {
                m_Attachments.push_back(imageView);
            }

            void AddAttchamnet(const Texture* texture)
            {
                m_Attachments.push_back(texture->GetDescriptorImageInfo().imageView);
            }
        };

    private:
        VkFramebuffer m_Framebuffer;
        VkExtent2D m_RenderArea;

        RenderPass* m_RenderPass;

    public:
        Framebuffer(RenderPass* renderPass, const VkExtent2D& renderArea);
        ~Framebuffer();

        bool SetupFramebuffer(const LogicalDevice* lDevice, const CreateInfo& createInfo);
    };

}}}