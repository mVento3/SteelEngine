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
        const std::vector<const char*> mc_ValidationLayers =
        {
            "VK_LAYER_KHRONOS_validation"
        };

        const bool mc_EnableValidationLayers = true;

        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData
        );

        VkInstance m_Instance;
        VkDebugUtilsMessengerEXT m_DebugMessenger;

        Interface::IWindow* m_Window;

        std::vector<const char*> GetSDL_Extensions();
        void PrintAvailableExtensions();
        bool CheckValidationLayerSupport();
        VkResult CreateDebugUtilsMessengerEXT(
            VkInstance instance,
            const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
            const VkAllocationCallbacks* pAllocator,
            VkDebugUtilsMessengerEXT* pDebugMessenger
        );

        Result CreateInstance();
        Result SetupDebugMessenger();

    public:
        VulkanRenderer(SteelEngine::Interface::IWindow* window);
        ~VulkanRenderer();

        Result Init() override;
        void Update() override;
        void Cleanup() override;
    };

}