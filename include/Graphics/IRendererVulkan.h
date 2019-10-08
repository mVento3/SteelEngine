#pragma once

#include "Graphics/IRenderer.h"

#include "Vulkan/vulkan.h"

#include "functional"

#include "Graphics/Vulkan/ICommandBuffer.h"

namespace SteelEngine { namespace Graphics {

    template <IRenderer::API API>
    struct IRendererAPI : public IRenderer
    {
        virtual void* GetDevice() const = 0;
        virtual VkQueue GetQueue() const = 0;
        virtual VkRenderPass GetRenderPass() const = 0;

        virtual void BindCommands(std::function<void(Vulkan::ICommandBuffer*)> func) = 0;
        virtual void BindUpdateUniforms(std::function<void()> func) = 0;
    };

}}