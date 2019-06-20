#pragma once

#include "Core/Result.h"

#include "Vulkan/vulkan.h"

#include "Rendering/Vulkan/Shader.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    class Renderer;

    class GraphicsPipeline
    {
    private:
        Shader* m_SomeShader;

        VkPipelineLayout m_PipelineLayout;

        VkPipeline m_GraphicsPipeline;

    public:
        GraphicsPipeline();
        ~GraphicsPipeline();

        Result Create(Renderer* renderer);

        inline VkPipeline GetPipeline() { return m_GraphicsPipeline; }
    };

}}}