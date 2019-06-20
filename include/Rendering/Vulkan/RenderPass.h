#pragma once

#include "Core/Result.h"

#include "Vulkan/vulkan.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    class Renderer;

    class RenderPass
    {
    private:
        VkRenderPass m_RenderPass;

    public:
        RenderPass();
        ~RenderPass();

        Result Create(Renderer* renderer);

        inline VkRenderPass GetRenderPass() { return m_RenderPass; }
    };

}}}