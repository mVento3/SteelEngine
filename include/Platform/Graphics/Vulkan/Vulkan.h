#pragma once

#include "Platform/Graphics/RenderDevice.h"

#include "Platform/Graphics/Vulkan/Texture.h"
#include "Platform/Graphics/Vulkan/CommandBuffer.h"
#include "Platform/Graphics/Vulkan/PhysicalDevice.h"
#include "Platform/Graphics/Vulkan/LogicalDevice.h"
#include "Platform/Graphics/Vulkan/Surface.h"
#include "Platform/Graphics/Vulkan/RenderPass.h"
#include "Platform/Graphics/Vulkan/Framebuffer.h"
#include "Platform/Graphics/Vulkan/Pipeline.h"
#include "Platform/Graphics/Vulkan/DescriptorPool.h"

#include "vulkan/vulkan.h"

#include "optional"
#include "set"

#include "glm/glm.hpp"

namespace SteelEngine { namespace Graphics {

    class Vulkan : public RenderDevice
    {
    public:
        struct Vertex
        {
            glm::vec2 m_Position;
            glm::vec2 m_TexCoord;

            static VkVertexInputBindingDescription GetBindingDescription()
            {
                VkVertexInputBindingDescription bindingDescription = {};

                bindingDescription.binding = 0;
                bindingDescription.stride = sizeof(Vertex);
                bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

                return bindingDescription;
            }

            static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions()
            {
                std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};

                attributeDescriptions[0].binding = 0;
                attributeDescriptions[0].location = 0;
                attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
                attributeDescriptions[0].offset = offsetof(Vertex, m_Position);

                attributeDescriptions[1].binding = 0;
                attributeDescriptions[1].location = 1;
                attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
                attributeDescriptions[1].offset = offsetof(Vertex, m_TexCoord);

                return attributeDescriptions;
            }
        };

        static const bool ms_EnableValidationLayers;
        static const std::vector<const char*> ms_DeviceExtensions;
        static const int MAX_FRAMES_IN_FLIGHT;

        const std::vector<Vertex> m_Vertices =
        {
            {{-0.5f, -0.5f}, {0.0f, 0.0f}},
            {{0.5f, -0.5f}, {1.0f, 0.0f}},
            {{0.5f, 0.5f}, {1.0f, 1.0f}},
            {{-0.5f, 0.5f}, {0.0f, 1.0f}}
        };

        const std::vector<uint16_t> m_Indices =
        {
            0, 1, 2, 2, 3, 0
        };

    private:
        IWindow* m_Window;

        VkInstance m_Instance;
        VkDebugUtilsMessengerEXT m_DebugMessenger;

        std::vector<const char*> m_Extensions;
        std::vector<VkSemaphore> m_ImageAvailableSemaphores;
        std::vector<VkSemaphore> m_RenderFinishedSemaphores;
        std::vector<VkFence> m_InFlightFences;
        std::vector<VkFence> m_ImagesInFlight;

        Platform::Vulkan::Pipeline* m_GraphicsPipeline;

        Platform::Vulkan::Framebuffer* m_ViewportFramebuffer;
        Platform::Vulkan::RenderPass* m_ViewportRenderPass;
        VkExtent2D m_ViewportExtent;

        Platform::Vulkan::Texture* m_ViewportTexture;

        Platform::Vulkan::DescriptorPool* m_Descriptor;

        size_t m_CurrentFrame;

        VkBuffer m_VertexBuffer;
        VkDeviceMemory m_VertexBufferMemory;

        VkBuffer m_IndexBuffer;
        VkDeviceMemory m_IndexBufferMemory;

        Platform::Vulkan::Texture* m_SomeTexture;

        VkCommandPool m_CommandPool;
        std::vector<VkCommandBuffer> m_CommandBuffers;
        Platform::Vulkan::CommandBuffer* m_CommandBuffer;

        std::function<void(void*, uint32_t)> m_EditorCommands;
        std::function<void()> m_Update;

    // Stared porting

        float m_ClearValues[4];

        bool CheckValidationLayerSupport(const std::vector<const char*>& validationLayers);
        bool CreateInstance();
        bool SetupDebugMessenger();
        bool SetupGraphicsPipeline();
        bool SetupCommandPool();
        bool SetupCommandBuffers();
        bool SetupSemaphores();
        bool SetupVertexBuffer();
        bool SetupIndexBuffer();
        bool SetupDescriptorSets();

        bool SetupViewport();

    public:
        Vulkan();
        ~Vulkan();

        Platform::Vulkan::PhysicalDevice* m_PDevice;
        Platform::Vulkan::LogicalDevice* m_LDevice;
        Platform::Vulkan::Surface* m_Surface;
        std::vector<VkImageView> m_SwapChainImageViews;

        VkDescriptorSet* m_EditorDescriptorSets;

        void Test(VkDescriptorImageInfo* info, VkDescriptorSet* desc);

        void** GetDescriptor() const override { return (void**)m_EditorDescriptorSets; }

    // Vulkan specific functions...

        bool CreateDescriptorPool(const VkDescriptorPoolCreateInfo& poolInfo, VkDescriptorPool& descriptorPool);
        bool CreateDescriptorSetLayout(const VkDescriptorSetLayoutCreateInfo& layoutInfo, VkDescriptorSetLayout& descriptorSetLayout);
        bool AllocateDescriptorSet(const VkDescriptorSetAllocateInfo& allocInfo, VkDescriptorSet& descriptorSet);
        bool AllocateDescriptorSets(const VkDescriptorSetAllocateInfo& allocInfo, std::vector<VkDescriptorSet>& descriptorSets);
        bool UpdateDescriptorSets(const std::vector<VkWriteDescriptorSet>& sets);
        bool CreatePipelineLayout(const VkPipelineLayoutCreateInfo& pipelineLayoutInfo, VkPipelineLayout& pipelineLayout);
        bool CreatePipeline(const VkGraphicsPipelineCreateInfo& pipelineCreateInfo, VkPipeline& pipeline);

        bool Initialize() override;
        bool PostInitialize() override;

        void SetVulkanExtensions(uint32_t enabledExtensionCount, const char** extensionNames) override;
        void SetWindow(IWindow* window) override;

        void SetEditorCommands(std::function<void(void*, uint32_t)> func) override { m_EditorCommands = func; }
        void SetUpdate(std::function<void()> func) override { m_Update = func; }

        void ClearColor(float r, float g, float b, float a) const override;
        void ClearColor(float r, float g, float b, float a) override;
        void Clear(ClearMask mask) const override;
        void DrawTriangles(uint32_t count) override;
    
    // Framebuffer

        void Setup(Graphics::IFramebuffer* framebuffer) const override;
        void Viewport(int x, int y, uint32_t width, uint32_t height) const override;
        void Viewport(const Graphics::IFramebuffer* framebuffer) const override;
        void Bind(const Graphics::IFramebuffer* framebuffer) const override;

        void GenerateFramebuffer(Graphics::IFramebuffer* framebuffer) const override;
        void FramebufferTexture2D(Graphics::IFramebuffer* framebuffer, Graphics::FramebufferAttachment attachment, bool* hasDepth, std::vector<uint32_t>& res) const override;

        void GenRenderbuffer(Graphics::IFramebuffer* framebuffer) const override;
        void BindRenderbuffer(Graphics::IFramebuffer* framebuffer) const override;
        void RenderbufferStorage(Graphics::IFramebuffer* framebuffer) const override;
        void FramebufferRenderbuffer(Graphics::IFramebuffer* framebuffer) const override;
        void DrawBuffers(const void* data, size_t count) const override;
        void DrawBuffers(const std::vector<uint32_t>& drawBuffers) const override;
        Graphics::FramebufferStatus CheckFramebufferStatus() const override;
        void BindFramebuffer(uint32_t id) const override;

    // Texture

        void GenerateTexture(Graphics::ITexture* texture) const override;
        void Bind(Graphics::ITexture* texture) const override;
        void TextureParameter(ParameterName name, ParameterValue value) const override;
        void TextureParameter(ParameterName name, Filter value) const override;
        void TextureImage2D(ITexture* texture, InternalFormat internalFormat, uint32_t width, uint32_t height, Format format, DataType type, const void* data = 0) const override;
    
    // Shader

        void Setup(IShader* shader, const char* vSourceData, const char* fSourceData) const override;
        void CheckShaderError(void* shader, ShaderFlag flag, bool isProgram, const std::string& errorMessage) const override;
        void* CreateShader(const char* sourceData, ShaderType type) const override;
        void Bind(IShader* shader) const override;
        ShaderUniform GetLocation(IShader* shader, const char* name) const override;
        void Set(IShader* shader, uint32_t index, float v) const override;
        void Set(IShader* shader, uint32_t index, int v) const override;
        void Set(IShader* shader, uint32_t index, const glm::vec2& v) const override;
        void Set(IShader* shader, uint32_t index, const glm::vec3& v) const override;
        void Set(IShader* shader, uint32_t index, const glm::mat4& v) const override;

    // Mesh

        void Begin(IMesh* mesh, uint32_t vbosCount) const override;
        void End(IMesh* mesh) const override;
        void PassVBO(IMesh* mesh, uint32_t vboIndex, const void* data, uint32_t countOfData, size_t sizeOfDataElement, uint8_t countOfElements) const override;
        void PassIBO(IMesh* mesh, uint32_t vboIndex, const void* data, uint32_t countOfData, size_t sizeOfDataElement) const override;
        void Draw(IMesh* mesh) const override;
    };

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    std::vector<char> readFile(const std::string& filename);
    VkShaderModule createShaderModule(Platform::Vulkan::LogicalDevice* lDevice, const std::vector<char>& code);
    // uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);
    VkCommandBuffer beginSingleTimeCommands(Platform::Vulkan::LogicalDevice* lDevice, VkCommandPool commandPool);
    void endSingleTimeCommands(Platform::Vulkan::LogicalDevice* lDevice, VkCommandPool commandPool, VkQueue graphicsQueue, VkCommandBuffer commandBuffer);
    bool createBuffer(Platform::Vulkan::PhysicalDevice* pDevice, Platform::Vulkan::LogicalDevice* lDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    void copyBuffer(Platform::Vulkan::LogicalDevice* lDevice, VkCommandPool commandPool, VkQueue graphicsQueue, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    void copyBufferToImage(Platform::Vulkan::LogicalDevice* lDevice, VkCommandPool commandPool, VkQueue graphicsQueue, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
    bool transitionImageLayout(Platform::Vulkan::LogicalDevice* lDevice, VkCommandPool commandPool, VkQueue graphicsQueue, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
    bool createImage(Platform::Vulkan::PhysicalDevice* pDevice, Platform::Vulkan::LogicalDevice* lDevice, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

}}