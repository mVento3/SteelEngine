#pragma once

#include "Rendering/IRenderer.h"

#include "RuntimeReflection/Macro.h"

#include "Vulkan/vulkan.h"

#include "VulkanRenderer.Generated.h"

namespace SteelEngine {

    SE_CLASS()
    class VulkanRenderer : public Interface::IRenderer
    {
        GENERATED_BODY
    private:
        VkInstance m_Instance;

    public:
        VulkanRenderer();
        ~VulkanRenderer();

        Result Init() override;
        void Update() override;
    };

}