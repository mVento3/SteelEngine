#pragma once

#include "Rendering/IRenderer.h"

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/ReflectionAttributes.h"

#include "Vulkan/vulkan.h"

#include "Renderer.Generated.h"

#include "Window/IWindow.h"
#include "Window/ResizeEvent.h"
#include "Window/MinimizedEvent.h"
#include "Window/MaximizedEvent.h"

#include "Rendering/Vulkan/PhysicalDevice.h"
#include "Rendering/Vulkan/LogicalDevice.h"
#include "Rendering/Vulkan/Surface.h"
#include "Rendering/Vulkan/SwapChain.h"
#include "Rendering/Vulkan/GraphicsPipeline.h"
#include "Rendering/Vulkan/RenderPass.h"
#include "Rendering/Vulkan/Framebuffer.h"
#include "Rendering/Vulkan/CommandPool.h"
#include "Rendering/Vulkan/Shader.h"

#define WIDTH 800
#define HEIGHT 600

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    SE_CLASS(SteelEngine::ReflectionAttribute::SE_RUNTIME_SERIALIZE)
    class Renderer : public Interface::IRenderer
    {
        GENERATED_BODY

        friend class PhysicalDevice;
        friend class LogicalDevice;
        friend class Surface;
        friend class SwapChain;
        friend class GraphicsPipeline;
        friend class RenderPass;
        friend class Framebuffer;
        friend class CommandPool;
        friend class VertexBuffer;
    public:
        const static std::vector<const char*> mc_ValidationLayers;
        const static bool mc_EnableValidationLayers;
        const static std::vector<const char*> mc_DeviceExtensions;
        const static int MAX_FRAMES_IN_FLIGHT;

        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData
        );

        const std::vector<Vertex> m_Vertices =
        {
            Vertex{{ 0.0f, -0.5f }, { 1.0f, 0.0f, 0.0f }},
            Vertex{{ 0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f }},
            Vertex{{ -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }}
        };

    private:
        VkInstance m_Instance;
        VkDebugUtilsMessengerEXT m_DebugMessenger;

        Interface::IWindow* m_Window;

        PhysicalDevice*     m_PhysicalDevice;
        LogicalDevice*      m_LogicalDevice;
        Surface*            m_Surface;
        SwapChain*          m_SwapChain;
        GraphicsPipeline*   m_GraphicsPipeline;
        RenderPass*         m_RenderPass;
        Framebuffer*        m_Framebuffer;
        CommandPool*        m_CommandPool;

        std::vector<VkSemaphore> m_ImageAvailableSemaphores;
        std::vector<VkSemaphore> m_RenderFinishedSemaphores;
        std::vector<VkFence> m_InFlightFences;
        std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStages;

        size_t m_CurrentFrame;
        bool m_FramebufferResized;
        bool m_WindowMinimized;

        Type::uint32 m_Width;
        Type::uint32 m_Height;

        Shader*         m_SomeShader;
        VertexBuffer*   m_Buffer;

        std::vector<const char*> GetSDL_Extensions();
        void PrintAvailableExtensions();
        bool CheckValidationLayerSupport();

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

        Result CreateInstance();
        Result SetupDebugMessenger();
        Result CreateSyncObjects();

    public:
        Renderer(SteelEngine::Interface::IWindow* window);
        ~Renderer();

        Result Init() override;
        void Update() override;
        void Cleanup() override;

        SE_METHOD()
        void RecreateSwapChain();

        void operator()(const ResizeEvent& event);
        void operator()(const MinimizedEvent& event);
        void operator()(const MaximizedEvent& event);
    };

}}}