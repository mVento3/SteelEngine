#pragma once

#include "Rendering/IRenderer.h"

#include "RuntimeReflection/Macro.h"

#include "Vulkan/vulkan.h"

#include "VulkanRenderer.Generated.h"

#include "Window/IWindow.h"

namespace SteelEngine {

    SE_CLASS()
    class VulkanRenderer : public Interface::IRenderer
    {
        GENERATED_BODY
    private:
        VkInstance m_Instance;

        Interface::IWindow* m_Window;

        std::vector<const char*> GetSDL_Extensions();

    public:
        VulkanRenderer(SteelEngine::Interface::IWindow* window);
        ~VulkanRenderer();

        Result Init() override;
        void Update() override;
    };

}