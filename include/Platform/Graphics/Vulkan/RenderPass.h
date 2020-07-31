#pragma once

#include "Platform/Graphics/Vulkan/LogicalDevice.h"

namespace SteelEngine { namespace Platform { namespace Vulkan {

    class CommandBuffer;
    class Framebuffer;
    class Pipeline;

    class RenderPass
    {
        friend class CommandBuffer;
        friend class Framebuffer;
        friend class Pipeline;
    public:
        struct CreateInfo
        {
            std::vector<VkAttachmentDescription> m_Attachments;
            std::vector<VkSubpassDescription> m_Subpasses;
            std::vector<VkSubpassDependency> m_Dependencies;

            void AddAttachment(const VkAttachmentDescription& description)
            {
                m_Attachments.push_back(description);
            }

            void AddSubpass(const VkSubpassDescription& description)
            {
                m_Subpasses.push_back(description);
            }

            void AddDependency(const VkSubpassDependency& dependency)
            {
                m_Dependencies.push_back(dependency);
            }
        };

    private:
        VkRenderPass m_RenderPass;

        LogicalDevice* m_LDevice;

    public:
        RenderPass(LogicalDevice* lDevice);
        ~RenderPass();

        bool SetupRenderPass(const CreateInfo& createInfo);

        inline const VkRenderPass& GetRenderPass() const { return m_RenderPass; }
    };

}}}