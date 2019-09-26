#pragma once

#include "Core/Result.h"

#include "Graphics/Vulkan/IProgramUserData.h"

#include "Vulkan/vulkan.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    class Device;

    struct IProgram
    {
        virtual Result Init(
            const Device* device,
            VkQueue queue,
            VkRenderPass renderPass,
            IProgramUserData* data) { return SE_NOT_IMPLEMENTED; }
        virtual void Render(VkCommandBuffer commandBuffer) { }
        virtual void DrawUI() { }
        virtual void UpdateBuffers() { }
    };

}}}