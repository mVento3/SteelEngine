#pragma once

#include "Vulkan/vulkan.h"

#include "Core/Result.h"

#include "vector"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    class Framebuffer;
    class VertexBuffer;
    class PhysicalDevice;
    class LogicalDevice;
    class Surface;
    class RenderPass;
    class SwapChain;
    class GraphicsPipeline;
    class DescriptorPool;

    class CommandPool
    {
        friend class Renderer;
        friend class Buffer;
    private:
        VkCommandPool m_CommandPool;

        std::vector<VkCommandBuffer> m_CommandBuffers;

    public:
        CommandPool();
        ~CommandPool();

        Result CreateCommandBuffers(
            const LogicalDevice& logicalDevice,
            const Framebuffer& framebuffer,
            const RenderPass& renderpass,
            const SwapChain& swapChain,
            const GraphicsPipeline& graphicsPipeline,
            const DescriptorPool& descriptorPool,
            std::vector<Buffer*> vertexVector,
            Buffer* indexBuffer
        );

        Result Create(
            const PhysicalDevice& physicalDevice,
            const LogicalDevice& logicalDevice,
            const Surface& surface
        );
        void Cleanup(const LogicalDevice& logicalDevice);
        void CleanupCommandBuffers(const LogicalDevice& logicalDevice);
    };

}}}