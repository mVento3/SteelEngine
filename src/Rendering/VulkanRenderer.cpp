#include "Rendering/VulkanRenderer.h"

namespace SteelEngine {

    VKAPI_ATTR VkBool32 VKAPI_CALL VulkanRenderer::DebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData)
    {
        printf("Validation Layer: %s\n", pCallbackData->pMessage);
        
        return VK_FALSE;
    }

    std::vector<const char*> VulkanRenderer::GetSDL_Extensions()
    {
        Type::uint32 count = 0;
        std::vector<const char*> res;

        m_Window->GetVulkanInstanceExtensions(
            &count,
            0
        );

        res.resize(count);

        m_Window->GetVulkanInstanceExtensions(
            &count,
            res.data()
        );

        if (mc_EnableValidationLayers)
        {
            res.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return res;
    }

    void VulkanRenderer::PrintAvailableExtensions()
    {
        Type::uint32 extensionCount = 0;

        vkEnumerateInstanceExtensionProperties(0, &extensionCount, 0);

        std::vector<VkExtensionProperties> extensions(extensionCount);

        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

        printf("Vulkan: Available extensions:\n");

        for(Type::uint32 i = 0; i < extensions.size(); i++)
        {
            printf("\t%s\n", extensions[i].extensionName);
        }

        printf("\n");
    }

    bool VulkanRenderer::CheckValidationLayerSupport()
    {
        uint32_t layerCount;

        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);

        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for(const char* layerName : mc_ValidationLayers)
        {
            bool layerFound = false;

            for(const auto& layerProperties : availableLayers)
            {
                if(strcmp(layerName, layerProperties.layerName) == 0)
                {
                    layerFound = true;

                    break;
                }
            }

            if(!layerFound)
            {
                return false;
            }
        }

        return true;
    }

    VkResult VulkanRenderer::CreateDebugUtilsMessengerEXT(
        VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkDebugUtilsMessengerEXT* pDebugMessenger)
    {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        
        if(func != nullptr)
        {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        }
        else
        {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    Result VulkanRenderer::CreateInstance()
    {
        VkApplicationInfo appInfo = {};

        appInfo.sType =                 VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName =      "Hello Triangle";
        appInfo.applicationVersion =    VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName =           "No Engine";
        appInfo.engineVersion =         VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion =            VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo = {};

        createInfo.sType =              VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo =   &appInfo;

        std::vector<const char*> extensions = GetSDL_Extensions();

        createInfo.enabledExtensionCount =      extensions.size();
        createInfo.ppEnabledExtensionNames =    extensions.data();

        if(mc_EnableValidationLayers)
        {
            createInfo.enabledLayerCount =      mc_ValidationLayers.size();
            createInfo.ppEnabledLayerNames =    mc_ValidationLayers.data();
        }
        else
        {
            createInfo.enabledLayerCount = 0;
        }

        if(vkCreateInstance(&createInfo, 0, &m_Instance) != VK_SUCCESS)
        {
            return SE_FALSE;
        }

        return SE_TRUE;
    }

    Result VulkanRenderer::SetupDebugMessenger()
    {
        if(!mc_EnableValidationLayers)
        {
            return SE_TRUE;
        }

        VkDebugUtilsMessengerCreateInfoEXT createInfo = {};

        createInfo.sType =              VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity =    VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType =        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback =    DebugCallback;
        createInfo.pUserData =          0;

        if(CreateDebugUtilsMessengerEXT(m_Instance, &createInfo, nullptr, &m_DebugMessenger) != VK_SUCCESS)
        {
            return SE_FALSE;
        }

        return SE_TRUE;
    }

    VulkanRenderer::VulkanRenderer(Interface::IWindow* window) :
        m_Window(window)
    {

    }

    VulkanRenderer::~VulkanRenderer()
    {

    }

    Result VulkanRenderer::Init()
    {
        if(mc_EnableValidationLayers && !CheckValidationLayerSupport())
        {
            return SE_FALSE;
        }

        if(CreateInstance() == SE_FALSE ||
            SetupDebugMessenger() == SE_FALSE)
        {
            return SE_FALSE;
        }

        PrintAvailableExtensions();

        return SE_TRUE;
    }

    void VulkanRenderer::Update()
    {
        
    }

    void VulkanRenderer::Cleanup()
    {
        vkDestroyInstance(m_Instance, 0);
    }

    void VulkanRenderer::operator()(const RecompiledEvent& event)
    {
        
    }

}