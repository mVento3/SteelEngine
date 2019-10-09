#pragma once

#include "Graphics/IRendererVulkan.h"

#include "RuntimeReflection/Macro.h"

#include "Core/ReflectionAttributes.h"

#include "Vulkan/vulkan.h"

#include "Window/IWindow.h"
#include "Window/ResizeEvent.h"
#include "Window/MinimizedEvent.h"
#include "Window/MaximizedEvent.h"

#include "Graphics/Vulkan/DeepLayer/Device.h"
#include "Graphics/Vulkan/DeepLayer/Validation.h"
#include "Graphics/Vulkan/DeepLayer/CommandPool.h"

#include "Graphics/Vulkan/PresentationLayer/Surface.h"
#include "Graphics/Vulkan/PresentationLayer/SwapChain.h"

#include "Graphics/Vulkan/Shader.h"
#include "Graphics/Vulkan/IndexBuffer.h"
#include "Graphics/Vulkan/Buffer.h"
#include "Graphics/Vulkan/VertexBuffer.h"
#include "Graphics/Vulkan/CommandBuffer.h"
#include "Graphics/Vulkan/DescriptorPool.h"

#include "Graphics/IEditor.h"
#include "Graphics/Vertex.h"

#include "vector"

#include "Graphics/Math/Camera.h"

#include "Input/Events/KeyUpEvent.h"
#include "Input/Events/KeyDownEvent.h"
#include "Input/Events/MouseMotionEvent.h"
#include "Input/Events/ChangeMousePositionEvent.h"

#include "RuntimeReflection/Variant.h"

#include "Graphics/Vulkan/Renderer.Generated.h"

#define WIDTH 800
#define HEIGHT 600

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    SE_CLASS(
        SteelEngine::ReflectionAttribute::RUNTIME_SERIALIZE
    )
    class Renderer : public IRendererAPI<IRenderer::API::VULKAN_API>
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
        friend class Validation;
    public:
        const static std::vector<const char*> mc_ValidationLayers;
        const static bool mc_EnableValidationLayers;
        const static std::vector<const char*> mc_DeviceExtensions;
        const static int MAX_FRAMES_IN_FLIGHT;

        const std::vector<Vertex> m_Vertices =
        {
            Vertex(glm::vec3(-0.5f, -0.5f, 0), glm::vec3(1, 0, 0)),
            Vertex(glm::vec3(0.5f, -0.5f, 0), glm::vec3(0, 1, 0)),
            Vertex(glm::vec3(0.5f, 0.5f, 0), glm::vec3(0, 0, 1)),
            Vertex(glm::vec3(-0.5f, 0.5f, 0), glm::vec3(1, 0, 1))
        };

        const std::vector<Vertex> m_Vertices2 =
        {
            Vertex(glm::vec3(-0.2f, -0.2f, 0), glm::vec3(0, 0, 1)),
            Vertex(glm::vec3(0.2f, -0.2f, 0), glm::vec3(0, 1, 0)),
            Vertex(glm::vec3(0.2f, 0.2f, 0), glm::vec3(1, 0, 0)),
            Vertex(glm::vec3(-0.2f, 0.2, 0), glm::vec3(1, 1, 0))
        };

        const std::vector<Type::uint16> m_Indices =
        {
            0, 1, 2, 2, 3, 0
        };

    private:
        VkInstance m_Instance;

        Device*         m_Device;
        Validation*     m_Validation;
        CommandPool*    m_CommandPool;
        CommandBuffer*  m_CommandBuffer;
        DescriptorPool* m_DescriptorPool;

        Surface*    m_Surface;
        SwapChain*  m_SwapChain;

        Camera m_Camera;

        VkRenderPass            m_RenderPass;
        VkDescriptorSetLayout   m_DescriptorSetLayout;
        VkPipelineLayout        m_PipelineLayout;
        VkPipeline              m_Pipeline;
        VkPipelineCache         m_PipelineCache;

        std::vector<VkFramebuffer>      m_Framebuffers;
        std::vector<VkCommandBuffer>    m_CommandBuffers;
        // std::vector<VkDescriptorSet>    m_DescriptorSets;

        IWindow* m_Window;

        std::vector<VkSemaphore> m_ImageAvailableSemaphores;
        std::vector<VkSemaphore> m_RenderFinishedSemaphores;
        std::vector<VkFence> m_InFlightFences;

        std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStages;

        std::vector<std::function<void(Vulkan::ICommandBuffer*)>> m_Commands;
        std::vector<std::function<void()>> m_UpdateBuffers;

        size_t m_CurrentFrame;
        bool m_FramebufferResized;
        bool m_WindowMinimized;
        bool m_Keys[256];
        bool m_RotateCamera;

        Type::uint32 m_Width;
        Type::uint32 m_Height;

        Shader*         m_SomeShader;
        VertexBuffer*   m_Buffer;
        VertexBuffer*   m_Buffer2;
        IndexBuffer*    m_IndexBuffer;

        Buffer* m_MVP_Uniform;
        Buffer* m_MVP_Uniform2;

        Transform m_Trans;
        Transform m_Trans2;

        std::vector<Buffer*> m_VertexArray;

        Variant* m_DeltaTimeVariant;

        std::vector<const char*> GetSDL_Extensions();
        void PrintAvailableExtensions();
        bool CheckValidationLayerSupport();

        Result CreateInstance();
        Result CreateSyncObjects();
        Result CreatePipeline();
        Result CreateFramebuffers();
        Result CreateCommandBuffers();

        Result GeneralInit();

    // Abstraction functions
        VkPipelineVertexInputStateCreateInfo VertexInput(
            VkVertexInputBindingDescription binding,
            const std::vector<VkVertexInputAttributeDescription>& descriptions
        );
        VkPipelineInputAssemblyStateCreateInfo InputAssembly(
            VkPrimitiveTopology topology,
            VkBool32 restartEnable
        );
        VkViewport Viewport(
            float x = 0,
            float y = 0,
            float width = 0,
            float height = 0,
            float minDepth = 0,
            float maxDepth = 1
        );
        VkRect2D Scissor(
            VkExtent2D extent,
            int32_t offsetX = 0,
            int32_t offsetY = 0
        );
        VkPipelineViewportStateCreateInfo ViewportState(
            VkViewport viewport,
            VkRect2D scissor
        );
        VkPipelineRasterizationStateCreateInfo Rasterizer(
            VkFrontFace frontFace,
            VkCullModeFlags cullMode,
            float lineWidth = 1.f,
            VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL
        );
        VkPipelineMultisampleStateCreateInfo Multisampling(
            VkBool32 shadingEnable = VK_FALSE,
            VkSampleCountFlagBits rasterizationSamples = VK_SAMPLE_COUNT_1_BIT
        );
        VkAttachmentDescription AttachmentDescription(
            VkFormat format
        );
        VkAttachmentReference AttachmentReference(
            uint32_t index,
            VkImageLayout layout
        );
        VkSubpassDescription Subpass(
            const std::vector<VkAttachmentReference>& attachmentRefs
        );
        VkSubpassDependency Dependency();
        VkRenderPassCreateInfo RenderPass(
            const std::vector<VkAttachmentDescription>& attachments,
            const std::vector<VkSubpassDescription>& subpasses,
            const std::vector<VkSubpassDependency>& dependencies
        );

    public:
        Renderer(IWindow* window);
        ~Renderer();

        Result Init() override;
        void Update() override;
        void Cleanup() override;

        SE_METHOD()
        void RecreateSwapChain();

        inline void* GetDevice() const override { return m_Device; }
        inline VkQueue GetQueue() const override { return m_Device->m_LogicalDevice->m_GraphicsQueue; }
        inline VkRenderPass GetRenderPass() const override { return m_RenderPass; }

        void BindCommands(std::function<void(Vulkan::ICommandBuffer*)> func) override;
        void BindUpdateUniforms(std::function<void()> func) override;

        void operator()(const ResizeEvent& event);
        void operator()(const MinimizedEvent& event);
        void operator()(const MaximizedEvent& event);
        void operator()(const KeyDownEvent& event);
        void operator()(const KeyUpEvent& event);
        void operator()(const MouseMotionEvent& event);
    };

}}}