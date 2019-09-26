#include "Graphics/Vulkan/DeepLayer/Validation.h"

#include "Graphics/Vulkan/Renderer.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    VkResult Validation::CreateDebugUtilsMessengerEXT(
        VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkDebugUtilsMessengerEXT* pDebugMessenger)
    {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        
        if(func != nullptr)
        {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        }
        else
        {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void Validation::DestroyDebugUtilsMessengerEXT(
            VkInstance instance,
            VkDebugUtilsMessengerEXT debugMessenger,
            const VkAllocationCallbacks* pAllocator)
    {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        
        if(func != nullptr)
        {
            func(instance, debugMessenger, pAllocator);
        }
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL Validation::DebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData)
    {
        printf("Validation Layer: %s\n", pCallbackData->pMessage);
        
        return VK_FALSE;
    }

    Validation::Validation()
    {

    }

    Validation::~Validation()
    {

    }

    void Validation::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
    {
        createInfo = {};

        createInfo.sType =              VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity =    VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType =        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback =    Validation::DebugCallback;
    }

    Result Validation::Init(const Renderer& renderer, bool enableValidation)
    {
        if(!enableValidation)
        {
            return SE_TRUE;
        }

        VkDebugUtilsMessengerCreateInfoEXT createInfo;

        PopulateDebugMessengerCreateInfo(createInfo);

        if(CreateDebugUtilsMessengerEXT(renderer.m_Instance, &createInfo, nullptr, &m_DebugMessenger) != VK_SUCCESS)
        {
            return SE_FALSE;
        }

        return SE_TRUE;
    }

    void Validation::Cleanup(const Renderer& renderer, bool enableValidation)
    {
        if(enableValidation)
        {
            DestroyDebugUtilsMessengerEXT(renderer.m_Instance, m_DebugMessenger, nullptr);
        }
    }

}}}