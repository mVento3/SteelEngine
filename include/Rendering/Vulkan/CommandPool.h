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

        Result CreateCommandBuffers();

    public:
        CommandPool(Renderer* renderer);
        ~CommandPool();

        Result Create();
        void Cleanup();
    };

}}}