#pragma once

#include "Vulkan/vulkan.h"

#include "Core/Result.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    class Renderer;

    class Validation
    {
    private:
        VkDebugUtilsMessengerEXT m_DebugMessenger;

        VkResult CreateDebugUtilsMessengerEXT(
            VkInstance instance,
            const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
            const VkAllocationCallbacks* pAllocator,
            VkDebugUtilsMessengerEXT* pDebugMessenger
        );
        void DestroyDebugUtilsMessengerEXT(
            VkInstance instance,
            VkDebugUtilsMessengerEXT debugMessenger,
            const VkAllocationCallbacks* pAllocator
        );

        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData
        );

    public:
        Validation();
        ~Validation();

        static void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

        Result Init(const Renderer& renderer, bool enableValidation);
        void Cleanup(const Renderer& renderer, bool enableValidation);
    };

}}}