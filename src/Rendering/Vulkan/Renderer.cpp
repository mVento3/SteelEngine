#include "Rendering/Vulkan/Renderer.h"

#include "algorithm"

#include "Event/GlobalEvent.h"

#undef max
#undef min

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    const bool Renderer::mc_EnableValidationLayers = true;
    const std::vector<const char*> Renderer::mc_ValidationLayers =
        {
            "VK_LAYER_KHRONOS_validation"
        };
    
    const std::vector<const char*> Renderer::mc_DeviceExtensions =
        {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
    
    const int Renderer::MAX_FRAMES_IN_FLIGHT = 2;

    void populateDebugMessengerCreateInfo(
        VkDebugUtilsMessengerCreateInfoEXT& createInfo)
    {
        createInfo = {};

        createInfo.sType =              VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity =    VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType =        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback =    Renderer::DebugCallback;
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL Renderer::DebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData)
    {
        printf("Validation Layer: %s\n", pCallbackData->pMessage);
        
        return VK_FALSE;
    }

    std::vector<const char*> Renderer::GetSDL_Extensions()
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

    void Renderer::PrintAvailableExtensions()
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

    bool Renderer::CheckValidationLayerSupport()
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

    VkResult Renderer::CreateDebugUtilsMessengerEXT(
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

    void Renderer::DestroyDebugUtilsMessengerEXT(
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

    Result Renderer::CreateInstance()
    {
        VkApplicationInfo appInfo = {};

        appInfo.sType =                 VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName =      "Hello Triangle";
        appInfo.applicationVersion =    VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName =           "SteelEngine";
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
            VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;

            populateDebugMessengerCreateInfo(debugCreateInfo);

            createInfo.enabledLayerCount =      mc_ValidationLayers.size();
            createInfo.ppEnabledLayerNames =    mc_ValidationLayers.data();
            createInfo.pNext =                  (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
        }
        else
        {
            createInfo.enabledLayerCount =  0;
            createInfo.pNext =              0;
        }

        if(vkCreateInstance(&createInfo, 0, &m_Instance) != VK_SUCCESS)
        {
            return SE_FALSE;
        }

        return SE_TRUE;
    }

    Result Renderer::SetupDebugMessenger()
    {
        if(!mc_EnableValidationLayers)
        {
            return SE_TRUE;
        }

        VkDebugUtilsMessengerCreateInfoEXT createInfo;

        populateDebugMessengerCreateInfo(createInfo);

        if(CreateDebugUtilsMessengerEXT(m_Instance, &createInfo, nullptr, &m_DebugMessenger) != VK_SUCCESS)
        {
            return SE_FALSE;
        }

        return SE_TRUE;
    }

    Result Renderer::CreateSyncObjects()
    {
        m_ImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        m_RenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        m_InFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

        VkSemaphoreCreateInfo semaphoreInfo = {};

        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo = {};

        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            if(vkCreateSemaphore(m_LogicalDevice->GetLogicalDevice(), &semaphoreInfo, nullptr, &m_ImageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(m_LogicalDevice->GetLogicalDevice(), &semaphoreInfo, nullptr, &m_RenderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(m_LogicalDevice->GetLogicalDevice(), &fenceInfo, nullptr, &m_InFlightFences[i]) != VK_SUCCESS)
            {
                return SE_FALSE;
            }
        }

        return SE_TRUE;
    }

    Renderer::Renderer(Interface::IWindow* window) :
        m_Window(window)
    {
        m_PhysicalDevice = new PhysicalDevice();
        m_LogicalDevice = new LogicalDevice();
        m_Surface = new Surface();
        m_SwapChain = new SwapChain();
        m_GraphicsPipeline = new GraphicsPipeline();
        m_RenderPass = new RenderPass();
        m_Framebuffer = new Framebuffer();
        m_CommandPool = new CommandPool();

        m_SomeShader = new Shader(m_LogicalDevice);
        m_Buffer = new VertexBuffer();

        m_CurrentFrame = 0;
        m_FramebufferResized = false;
        m_WindowMinimized = false;

        window->GetWindowSize(&m_Width, &m_Height);
    }

    Renderer::~Renderer()
    {

    }

    Result Renderer::Init()
    {
        if(mc_EnableValidationLayers && !CheckValidationLayerSupport())
        {
            return SE_FALSE;
        }

        if(CreateInstance() == SE_FALSE)
        {
            return SE_FALSE;
        }

        PrintAvailableExtensions();

        if(SetupDebugMessenger() == SE_FALSE)
        {
            return SE_FALSE;
        }

        if(m_Surface->Create(this, m_Window) == SE_FALSE)
        {
            return SE_FALSE;
        }

        if(m_PhysicalDevice->PickPhysicalDevice(this) == SE_FALSE)
        {
            return SE_FALSE;
        }

        if(m_LogicalDevice->Create(*m_PhysicalDevice, *m_Surface) == SE_FALSE)
        {
            return SE_FALSE;
        }

        if(m_SwapChain->Create(*m_PhysicalDevice, *m_LogicalDevice, *m_Surface) == SE_FALSE)
        {
            return SE_FALSE;
        }

        if(m_RenderPass->Create(*m_LogicalDevice, *m_SwapChain) == SE_FALSE)
        {
            return SE_FALSE;
        }

        m_SomeShader->LoadShader("", m_ShaderStages);
        m_Buffer->CreateVertexBuffer(this, sizeof(m_Vertices[0]) * m_Vertices.size(), m_Vertices.data());

        if(m_GraphicsPipeline->Create(*m_LogicalDevice, *m_SwapChain, *m_RenderPass, m_ShaderStages) == SE_FALSE)
        {
            return SE_FALSE;
        }

        m_SomeShader->Destroy();

        if(m_Framebuffer->Create(*m_LogicalDevice, *m_SwapChain, *m_RenderPass) == SE_FALSE)
        {
            return SE_FALSE;
        }

        if(m_CommandPool->Create(
            *m_PhysicalDevice,
            *m_LogicalDevice,
            *m_Surface) == SE_FALSE)
        {
            return SE_FALSE;
        }

        if(m_CommandPool->CreateCommandBuffers(
            *m_LogicalDevice,
            *m_Framebuffer,
            *m_RenderPass,
            *m_SwapChain,
            m_Buffer,
            m_GraphicsPipeline->GetPipeline()) == SE_FALSE)
        {
            return SE_FALSE;
        }

        if(CreateSyncObjects() == SE_FALSE)
        {
            return SE_FALSE;
        }

        Event::GlobalEvent::Add<ResizeEvent>(this);
        Event::GlobalEvent::Add<MinimizedEvent>(this);
        Event::GlobalEvent::Add<MaximizedEvent>(this);

        return SE_TRUE;
    }

    void Renderer::Update()
    {
        vkWaitForFences(m_LogicalDevice->GetLogicalDevice(), 1, &m_InFlightFences[m_CurrentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());

        uint32_t imageIndex;
        
        VkResult result = vkAcquireNextImageKHR(m_LogicalDevice->GetLogicalDevice(), m_SwapChain->m_SwapChain, std::numeric_limits<uint64_t>::max(), m_ImageAvailableSemaphores[m_CurrentFrame], VK_NULL_HANDLE, &imageIndex);
    
        if(result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            RecreateSwapChain();

            return;
        }
        else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            throw std::runtime_error("Failed to acquire swap chain image!");
        }

        VkSubmitInfo submitInfo = {};

        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = { m_ImageAvailableSemaphores[m_CurrentFrame] };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores =    waitSemaphores;
        submitInfo.pWaitDstStageMask =  waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers =    &m_CommandPool->m_CommandBuffers[imageIndex];

        VkSemaphore signalSemaphores[] = { m_RenderFinishedSemaphores[m_CurrentFrame] };

        submitInfo.signalSemaphoreCount =   1;
        submitInfo.pSignalSemaphores =      signalSemaphores;

        vkResetFences(m_LogicalDevice->GetLogicalDevice(), 1, &m_InFlightFences[m_CurrentFrame]);

        if(vkQueueSubmit(m_LogicalDevice->m_GraphicsQueue, 1, &submitInfo, m_InFlightFences[m_CurrentFrame]) != VK_SUCCESS)
        {
            return;
        }

        VkPresentInfoKHR presentInfo = {};

        presentInfo.sType =                 VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount =    1;
        presentInfo.pWaitSemaphores =       signalSemaphores;

        VkSwapchainKHR swapChains[] = { m_SwapChain->m_SwapChain };

        presentInfo.swapchainCount =    1;
        presentInfo.pSwapchains =       swapChains;
        presentInfo.pImageIndices =     &imageIndex;
        presentInfo.pResults =          nullptr; // Optional

        result = vkQueuePresentKHR(m_LogicalDevice->m_PresentQueue, &presentInfo);

        if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_FramebufferResized)
        {
            m_FramebufferResized = false;

            RecreateSwapChain();
        }
        else if(result != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to present swap chain image!");
        }

        m_CurrentFrame = (m_CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    void Renderer::Cleanup()
    {
        vkDeviceWaitIdle(m_LogicalDevice->GetLogicalDevice());

        m_Framebuffer->Cleanup(*m_LogicalDevice);
        m_CommandPool->Cleanup(*m_LogicalDevice);
        m_GraphicsPipeline->Cleanup(*m_LogicalDevice);
        m_RenderPass->Cleanup(*m_LogicalDevice);
        m_SwapChain->Cleanup(*m_LogicalDevice);

        for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            vkDestroySemaphore(m_LogicalDevice->GetLogicalDevice(), m_RenderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(m_LogicalDevice->GetLogicalDevice(), m_ImageAvailableSemaphores[i], nullptr);
            vkDestroyFence(m_LogicalDevice->GetLogicalDevice(), m_InFlightFences[i], nullptr);
        }

        vkDestroyDevice(m_LogicalDevice->GetLogicalDevice(), nullptr);

        if(mc_EnableValidationLayers)
        {
            DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);
        }

        m_Surface->Cleanup(this);
        vkDestroyInstance(m_Instance, 0);
    }

    void Renderer::RecreateSwapChain()
    {
        if(m_WindowMinimized)
        {
            return;
        }

        vkDeviceWaitIdle(m_LogicalDevice->GetLogicalDevice());

        m_Framebuffer->Cleanup(*m_LogicalDevice);
        m_CommandPool->CleanupCommandBuffers(*m_LogicalDevice);
        m_GraphicsPipeline->Cleanup(*m_LogicalDevice);
        m_RenderPass->Cleanup(*m_LogicalDevice);
        m_SwapChain->Cleanup(*m_LogicalDevice);

        m_SwapChain->Create(*m_PhysicalDevice, *m_LogicalDevice, *m_Surface);
        m_RenderPass->Create(*m_LogicalDevice, *m_SwapChain);

        m_ShaderStages.clear();
        m_SomeShader->LoadShader("", m_ShaderStages);
        m_GraphicsPipeline->Create(*m_LogicalDevice, *m_SwapChain, *m_RenderPass, m_ShaderStages);
        m_SomeShader->Destroy();
        m_Framebuffer->Create(*m_LogicalDevice, *m_SwapChain, *m_RenderPass);
        m_CommandPool->CreateCommandBuffers(
            *m_LogicalDevice,
            *m_Framebuffer,
            *m_RenderPass,
            *m_SwapChain,
            m_Buffer,
            m_GraphicsPipeline->GetPipeline()
        );
    }

    void Renderer::operator()(const RecompiledEvent& event)
    {
        
    }

    void Renderer::operator()(const ResizeEvent& event)
    {
        m_FramebufferResized = true;

        m_Width = event.m_Width;
        m_Height = event.m_Height;
    }

    void Renderer::operator()(const MinimizedEvent& event)
    {
        m_WindowMinimized = true;
    }

    void Renderer::operator()(const MaximizedEvent& event)
    {
        m_WindowMinimized = false;
    }

}}}