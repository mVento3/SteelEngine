#pragma once

#include "Vulkan/vulkan.h"

#include "Core/Result.h"

#include "vector"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    class Renderer;

    class CommandPool
    {
        friend class Renderer;
    private:
        Renderer* m_Renderer;

        VkCommandPool m_CommandPool;

        std::vector<VkCommandBuffer> m_CommandBuffers;

    public:
        CommandPool(Renderer* renderer);
        ~CommandPool();

        Result CreateCommandBuffers();

        Result Create();
        void Cleanup();
        void CleanupCommandBuffers();
    };

}}}