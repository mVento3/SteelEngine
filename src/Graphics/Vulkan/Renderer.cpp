#include "Graphics/Vulkan/Renderer.h"

#include "algorithm"

#include "Event/GlobalEvent.h"

#include "Graphics/UniformBufferObject.h"

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "RuntimeReflection/Reflection.h"

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

        if(mc_EnableValidationLayers)
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

            Validation::PopulateDebugMessengerCreateInfo(debugCreateInfo);

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
            if(m_Device->CreateSemaphore(&semaphoreInfo, m_ImageAvailableSemaphores[i]) == SE_FALSE ||
                m_Device->CreateSemaphore(&semaphoreInfo, m_RenderFinishedSemaphores[i]) == SE_FALSE ||
                m_Device->CreateFence(&fenceInfo, m_InFlightFences[i]) == SE_FALSE)
            {
                return SE_FALSE;
            }
        }

        return SE_TRUE;
    }

    Result Renderer::GeneralInit()
    {
        if(mc_EnableValidationLayers && !CheckValidationLayerSupport())
        {
            printf("Disabled validation layers or check failed!\n");

            return SE_FALSE;
        }

        if(CreateInstance() == SE_FALSE)
        {
            printf("Cretate instance failed!\n");

            return SE_FALSE;
        }

        PrintAvailableExtensions();

        if(m_Validation->Init(*this, mc_EnableValidationLayers) == SE_FALSE)
        {
            printf("Validation init failed!\n");

            return SE_FALSE;
        }

        if(m_Surface->Create(this, m_Window) == SE_FALSE)
        {
            printf("Surface creation failed!\n");

            return SE_FALSE;
        }

        if(m_Device->Setup(m_Instance, m_Surface) == SE_FALSE)
        {
            printf("Device setup failed!\n");

            return SE_FALSE;
        }

        if(m_Device->CreateSwapChain(m_Surface, m_SwapChain) == SE_FALSE)
        {
            printf("Swap chain creation failed!\n");

            return SE_FALSE;
        }

        printf("General vulkan init success!\n");

        return SE_TRUE;
    }

// Abstraction functions
    VkPipelineVertexInputStateCreateInfo Renderer::VertexInput(
        VkVertexInputBindingDescription binding,
        const std::vector<VkVertexInputAttributeDescription>& descriptions)
    {
        VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};

        vertexInputInfo.sType =                             VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount =     1;
        vertexInputInfo.pVertexBindingDescriptions =        &binding;
        vertexInputInfo.vertexAttributeDescriptionCount =   descriptions.size();
        vertexInputInfo.pVertexAttributeDescriptions =      descriptions.data();

        return vertexInputInfo;
    }

    VkPipelineInputAssemblyStateCreateInfo Renderer::InputAssembly(
        VkPrimitiveTopology topology,
        VkBool32 restartEnable)
    {
        VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};

        inputAssembly.sType =                   VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology =                topology;
        inputAssembly.primitiveRestartEnable =  restartEnable;

        return inputAssembly;
    }

    VkViewport Renderer::Viewport(
        float x,
        float y,
        float width,
        float height,
        float minDepth,
        float maxDepth)
    {
        VkViewport viewport = {};

        viewport.x =        x;
        viewport.y =        y;

        if(width == 0)
        {
            viewport.width = (float)m_SwapChain->m_SwapChainExtent.width;
        }

        if(height == 0)
        {
            viewport.height = (float)m_SwapChain->m_SwapChainExtent.height;
        }

        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        return viewport;
    }

    VkRect2D Renderer::Scissor(
        VkExtent2D extent,
        int32_t offsetX,
        int32_t offsetY)
    {
        VkRect2D scissor = {};

        scissor.offset = { offsetX, offsetY };
        scissor.extent = extent;

        return scissor;
    }

    VkPipelineViewportStateCreateInfo Renderer::ViewportState(
        VkViewport viewport,
        VkRect2D scissor)
    {
        VkPipelineViewportStateCreateInfo viewportState = {};

        viewportState.sType =           VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount =   1;
        viewportState.pViewports =      &viewport;
        viewportState.scissorCount =    1;
        viewportState.pScissors =       &scissor;

        return viewportState;
    }

    VkPipelineRasterizationStateCreateInfo Renderer::Rasterizer(
        VkFrontFace frontFace,
        VkCullModeFlags cullMode,
        float lineWidth,
        VkPolygonMode polygonMode)
    {
        VkPipelineRasterizationStateCreateInfo rasterizer = {};

        rasterizer.sType =                      VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable =           VK_FALSE;
        rasterizer.rasterizerDiscardEnable =    VK_FALSE;
        rasterizer.polygonMode =                polygonMode;
        rasterizer.lineWidth =                  lineWidth;
        rasterizer.cullMode =                   cullMode;
        rasterizer.frontFace =                  frontFace;
        rasterizer.depthBiasEnable =            VK_FALSE;

        return rasterizer;
    }

    VkPipelineMultisampleStateCreateInfo Renderer::Multisampling(
        VkBool32 shadingEnable,
        VkSampleCountFlagBits rasterizationSamples)
    {
        VkPipelineMultisampleStateCreateInfo multisampling = {};

        multisampling.sType =                   VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable =     shadingEnable;
        multisampling.rasterizationSamples =    rasterizationSamples;

        return multisampling;
    }

    VkAttachmentDescription Renderer::AttachmentDescription(
        VkFormat format)
    {
        VkAttachmentDescription attachment = {};

        attachment.format =            format;
        attachment.samples =           VK_SAMPLE_COUNT_1_BIT;
        attachment.loadOp =            VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachment.storeOp =           VK_ATTACHMENT_STORE_OP_STORE;
        attachment.stencilLoadOp =     VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachment.stencilStoreOp =    VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachment.initialLayout =     VK_IMAGE_LAYOUT_UNDEFINED;
        attachment.finalLayout =       VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        return attachment;
    }

    VkAttachmentReference Renderer::AttachmentReference(
        uint32_t index,
        VkImageLayout layout)
    {
        VkAttachmentReference attachmentRef = {};

        attachmentRef.attachment = index;
        attachmentRef.layout =     layout;

        return attachmentRef;
    }

    VkSubpassDescription Renderer::Subpass(
        const std::vector<VkAttachmentReference>& attachmentRefs)
    {
        VkSubpassDescription subpass = {};

        subpass.pipelineBindPoint =     VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount =  attachmentRefs.size();
        subpass.pColorAttachments =     attachmentRefs.data();

        return subpass;
    }

    VkSubpassDependency Renderer::Dependency()
    {
        VkSubpassDependency dependency = {};

        dependency.srcSubpass =     VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass =     0;
        dependency.srcStageMask =   VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask =  0;
        dependency.dstStageMask =   VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask =  VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        return dependency;
    }

    VkRenderPassCreateInfo Renderer::RenderPass(
        const std::vector<VkAttachmentDescription>& attachments,
        const std::vector<VkSubpassDescription>& subpasses,
        const std::vector<VkSubpassDependency>& dependencies)
    {
        VkRenderPassCreateInfo renderPassInfo = {};

        renderPassInfo.sType =              VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount =    attachments.size();
        renderPassInfo.pAttachments =       attachments.data();
        renderPassInfo.subpassCount =       subpasses.size();
        renderPassInfo.pSubpasses =         subpasses.data();
        renderPassInfo.dependencyCount =    dependencies.size();
        renderPassInfo.pDependencies =      dependencies.data();

        return renderPassInfo;
    }

    Renderer::Renderer(IWindow* window) :
        m_Window(window)
    {
        m_Device = new Device();
        m_Validation = new Validation();
        m_CommandPool = new CommandPool();

        m_Surface = new Surface();
        m_SwapChain = new SwapChain();

        m_SomeShader = new Shader();
        m_Buffer = new VertexBuffer();
        m_Buffer2 = new VertexBuffer();
        m_IndexBuffer = new IndexBuffer();

        m_CurrentFrame = 0;
        m_FramebufferResized = false;
        m_WindowMinimized = false;

        // m_ImGUI = new ImGUI_Program();
    }

    Renderer::~Renderer()
    {

    }

    Result Renderer::Init()
    {
        m_Window->GetWindowSize(m_Width, m_Height);

        if(GeneralInit() == SE_FALSE)
        {
            printf("General init failed!\n");

            return SE_FALSE;
        }

        std::vector<VkAttachmentDescription> attachments =
        {
            AttachmentDescription(m_SwapChain->m_SwapChainImageFormat)
        };

        std::vector<VkAttachmentReference> attachmentRefs =
        {
            AttachmentReference(0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
        };

        std::vector<VkSubpassDescription> subpasses =
        {
            Subpass(attachmentRefs)
        };

        std::vector<VkSubpassDependency> dependencies =
        {
            Dependency()
        };

        VkRenderPassCreateInfo renderPassInfo =
            RenderPass(attachments, subpasses, dependencies);

        if(m_Device->CreateRenderPass(
            &renderPassInfo,
            m_RenderPass) == SE_FALSE)
        {
            printf("Render pass creation failed!\n");

            return SE_FALSE;
        }

        if(m_Device->LoadShader(
            "testShader",
            m_SomeShader,
            m_ShaderStages) == SE_FALSE)
        {
            printf("Failed to load shader!\n");

            return SE_FALSE;
        }

        std::vector<VkDescriptorSetLayoutBinding> uboLayoutBindings;

        VkDescriptorSetLayoutBinding uboLayoutBinding = {};

        uboLayoutBinding.binding =              0;
        uboLayoutBinding.descriptorType =       VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding.descriptorCount =      1;
        uboLayoutBinding.stageFlags =           VK_SHADER_STAGE_VERTEX_BIT;
        uboLayoutBinding.pImmutableSamplers =   nullptr; // Optional

        uboLayoutBindings.push_back(uboLayoutBinding);

        VkDescriptorSetLayoutCreateInfo layoutInfo = {};

        layoutInfo.sType =          VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount =   uboLayoutBindings.size();
        layoutInfo.pBindings =      uboLayoutBindings.data();

        if(m_Device->CreateDescriptorSetLayout(
            &layoutInfo,
            m_DescriptorSetLayout) == SE_FALSE)
        {
            printf("Descriptor set creation failed!\n");

            return SE_FALSE;
        }

        auto bindingDescription = Vertex::GetBindingDescription();
        auto attributeDescription = Vertex::GetAttributeDescriptions();

        VkPipelineVertexInputStateCreateInfo vertexInputInfo =
            VertexInput(bindingDescription, attributeDescription);

        VkPipelineInputAssemblyStateCreateInfo inputAssembly =
            InputAssembly(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE);

        VkViewport viewport = Viewport();
        VkRect2D scissor = Scissor(m_SwapChain->m_SwapChainExtent);

        VkPipelineViewportStateCreateInfo viewportState =
            ViewportState(viewport, scissor);

        VkPipelineRasterizationStateCreateInfo rasterizer =
            Rasterizer(VK_FRONT_FACE_COUNTER_CLOCKWISE, VK_CULL_MODE_BACK_BIT);

        VkPipelineMultisampleStateCreateInfo multisampling =
            Multisampling();

        VkPipelineColorBlendAttachmentState colorBlendAttachment = {};

        colorBlendAttachment.colorWriteMask =   VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable =      VK_FALSE;

        VkPipelineColorBlendStateCreateInfo colorBlending = {};

        colorBlending.sType =           VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable =   VK_FALSE;
        colorBlending.logicOp =         VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments =    &colorBlendAttachment;

        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;

        VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};

        pipelineLayoutInfo.sType =          VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 1;
        pipelineLayoutInfo.pSetLayouts =    &m_DescriptorSetLayout;

        if(m_Device->CreatePipelineLayout(
            &pipelineLayoutInfo,
            m_PipelineLayout) == SE_FALSE)
        {
            return SE_FALSE;
        }

        VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};

		pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;

        if(m_Device->CreatePipelineCache(
            &pipelineCacheCreateInfo,
            m_PipelineCache) == SE_FALSE)
        {
            return SE_FALSE;
        }

        VkGraphicsPipelineCreateInfo pipelineInfo = {};

        pipelineInfo.sType =                VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount =           m_ShaderStages.size();
        pipelineInfo.pStages =              m_ShaderStages.data();
        pipelineInfo.pVertexInputState =    &vertexInputInfo;
        pipelineInfo.pInputAssemblyState =  &inputAssembly;
        pipelineInfo.pViewportState =       &viewportState;
        pipelineInfo.pRasterizationState =  &rasterizer;
        pipelineInfo.pMultisampleState =    &multisampling;
        pipelineInfo.pDepthStencilState =   nullptr; // Optional
        pipelineInfo.pColorBlendState =     &colorBlending;
        pipelineInfo.pDynamicState =        nullptr; // Optional
        pipelineInfo.layout =               m_PipelineLayout;
        pipelineInfo.renderPass =           m_RenderPass;
        pipelineInfo.subpass =              0;
        pipelineInfo.basePipelineHandle =   VK_NULL_HANDLE; // Optional
        pipelineInfo.basePipelineIndex =    -1; // Optional

        if(m_Device->CreatePipelines(
            { pipelineInfo },
            m_Pipeline,
            m_PipelineCache) == SE_FALSE)
        {
            return SE_FALSE;
        }

        m_Device->DestroyShader(m_SomeShader);

        m_Framebuffers.resize(m_SwapChain->m_SwapChainImageViews.size());

        for(size_t i = 0; i < m_SwapChain->m_SwapChainImageViews.size(); i++)
        {
            VkImageView attachments[] =
            {
                m_SwapChain->m_SwapChainImageViews[i]
            };

            VkFramebufferCreateInfo framebufferInfo = {};

            framebufferInfo.sType =             VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass =        m_RenderPass;
            framebufferInfo.attachmentCount =   1;
            framebufferInfo.pAttachments =      attachments;
            framebufferInfo.width =             m_SwapChain->m_SwapChainExtent.width;
            framebufferInfo.height =            m_SwapChain->m_SwapChainExtent.height;
            framebufferInfo.layers =            1;

            if(m_Device->CreateFramebuffer(
                &framebufferInfo,
                m_Framebuffers[i]) == SE_FALSE)
            {
                return SE_FALSE;
            }
        }

        if(m_Device->CreateCommandPool(m_CommandPool) == SE_FALSE)
        {
            return SE_FALSE;
        }

        m_Buffer->CreateVertexBuffer(
            m_Device,
            m_Vertices.data(),
            m_Vertices.size()
        );
        m_Buffer2->CreateVertexBuffer(
            m_Device,
            m_Vertices2.data(),
            m_Vertices2.size()
        );
        m_IndexBuffer->CreateIndexBuffer(
            m_Device,
            m_Indices.data(),
            m_Indices.size()
        );

        m_VertexArray = { m_Buffer, m_Buffer2 };

        m_Uniforms.resize(m_SwapChain->m_SwapChainImages.size());

        for(size_t i = 0; i < m_SwapChain->m_SwapChainImages.size(); i++)
        {
            m_Uniforms[i] = new Buffer();

            m_Uniforms[i]->CreateBuffer(
                m_Device,
                sizeof(UniformBufferObject),
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
            );
        }

        VkDescriptorPoolSize poolSize = {};

        poolSize.type =             VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSize.descriptorCount =  m_SwapChain->m_SwapChainImages.size();

        VkDescriptorPoolCreateInfo descriptorPoolInfo = {};

        descriptorPoolInfo.sType =            VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        descriptorPoolInfo.poolSizeCount =    1;
        descriptorPoolInfo.pPoolSizes =       &poolSize;
        descriptorPoolInfo.maxSets =          m_SwapChain->m_SwapChainImages.size();

        if(m_Device->CreateDescriptorPool(
            &descriptorPoolInfo,
            m_DescriptorPool) == SE_FALSE)
        {
            return SE_FALSE;
        }

        std::vector<VkDescriptorSetLayout> layouts(m_SwapChain->m_SwapChainImages.size(), m_DescriptorSetLayout);
        VkDescriptorSetAllocateInfo descriptorAllocInfo = {};

        descriptorAllocInfo.sType =               VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        descriptorAllocInfo.descriptorPool =      m_DescriptorPool;
        descriptorAllocInfo.descriptorSetCount =  m_SwapChain->m_SwapChainImages.size();
        descriptorAllocInfo.pSetLayouts =         layouts.data();

        m_DescriptorSets.resize(m_SwapChain->m_SwapChainImages.size());

        if(m_Device->AllocateDescriptorSets(
            &descriptorAllocInfo,
            m_DescriptorSets) == SE_FALSE)
        {
            return SE_FALSE;
        }

        for(size_t i = 0; i < m_SwapChain->m_SwapChainImages.size(); i++)
        {
            VkDescriptorBufferInfo bufferInfo = {};

            bufferInfo.buffer = m_Uniforms[i]->GetBuffer();
            bufferInfo.offset = 0;
            bufferInfo.range =  sizeof(UniformBufferObject);

            VkWriteDescriptorSet descriptorWrite = {};

            descriptorWrite.sType =             VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite.dstSet =            m_DescriptorSets[i];
            descriptorWrite.dstBinding =        0;
            descriptorWrite.dstArrayElement =   0;
            descriptorWrite.descriptorType =    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrite.descriptorCount =   1;
            descriptorWrite.pBufferInfo =       &bufferInfo;
            descriptorWrite.pImageInfo =        nullptr; // Optional
            descriptorWrite.pTexelBufferView =  nullptr; // Optional

            m_Device->UpdateDescriptorSets(
                { descriptorWrite }
            );
        }

        m_CommandBuffers.resize(m_Framebuffers.size());

        if(m_Device->AllocateCommandBuffers(
            m_CommandBuffers) == SE_FALSE)
        {
            return SE_FALSE;
        }

    // Not here!!!!!
        // ProgramUserData* udata = new ProgramUserData(m_Window);

        // udata->m_Editor = m_Editor;

        // if(m_ImGUI->Init(
        //     m_Device,
        //     m_Device->m_LogicalDevice->m_GraphicsQueue,
        //     m_RenderPass,
        //     udata) == SE_FALSE)
        // {
        //     return SE_FALSE;
        // }
    // ----------------------------------------------------------

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
        // m_ImGUI->DrawUI();
        // m_ImGUI->UpdateBuffers();

        m_Device->WaitForFences(
            { m_InFlightFences[m_CurrentFrame] },
            VK_TRUE,
            std::numeric_limits<uint64_t>::max()
        );

        Type::uint32 frameIndex;

        VkResult result = m_Device->AcquireNextImage(
            frameIndex,
            m_ImageAvailableSemaphores[m_CurrentFrame],
            VK_NULL_HANDLE,
            std::numeric_limits<uint64_t>::max()
        );

        if(result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            RecreateSwapChain();

            return;
        }
        else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            throw std::runtime_error("Failed to acquire swap chain image!");
        }

        VkCommandBufferBeginInfo beginInfo = {};

        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

        if(vkBeginCommandBuffer(m_CommandBuffers[frameIndex], &beginInfo) != VK_SUCCESS)
        {
            return;
        }

        VkRenderPassBeginInfo renderPassInfo = {};

        renderPassInfo.sType =              VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass =         m_RenderPass;
        renderPassInfo.framebuffer =        m_Framebuffers[frameIndex];
        renderPassInfo.renderArea.offset =  { 0, 0 };
        renderPassInfo.renderArea.extent =  m_SwapChain->m_SwapChainExtent;

        VkClearValue clearColor = { 0.0f, 0.15f, 0.3f, 1.0f };

        renderPassInfo.clearValueCount =    1;
        renderPassInfo.pClearValues =       &clearColor;

        vkCmdBeginRenderPass(m_CommandBuffers[frameIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            vkCmdBindPipeline(m_CommandBuffers[frameIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline);

            for(Buffer* data : m_VertexArray)
            {
                VkBuffer buff = data->GetBuffer();
                VkDeviceSize offsets[] = { 0 };

                vkCmdBindVertexBuffers(m_CommandBuffers[frameIndex], 0, 1, &buff, offsets);
                vkCmdBindIndexBuffer(m_CommandBuffers[frameIndex], m_IndexBuffer->GetBuffer(), 0, VK_INDEX_TYPE_UINT16);
                vkCmdBindDescriptorSets(m_CommandBuffers[frameIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, m_PipelineLayout, 0, 1, &m_DescriptorSets[frameIndex], 0, nullptr);
                vkCmdDrawIndexed(m_CommandBuffers[frameIndex], m_IndexBuffer->GetCount(), 1, 0, 0, 0);
            }

            // m_ImGUI->Render(m_CommandBuffers[frameIndex]);

        vkCmdEndRenderPass(m_CommandBuffers[frameIndex]);

        if(vkEndCommandBuffer(m_CommandBuffers[frameIndex]) != VK_SUCCESS)
        {
            return;
        }

        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        UniformBufferObject ubo;

        ubo.m_Model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.m_View = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.m_Projection = glm::perspective(glm::radians(70.0f), (float)m_Width / (float)m_Height, 0.01f, 1000.0f);
        ubo.m_Projection[1][1] *= -1;

        m_Device->Copy(
            m_Uniforms[frameIndex]->GetMemory(),
            &ubo
        );

        VkSubmitInfo submitInfo = {};

        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = { m_ImageAvailableSemaphores[m_CurrentFrame] };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores =    waitSemaphores;
        submitInfo.pWaitDstStageMask =  waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers =    &m_CommandBuffers[frameIndex];

        VkSemaphore signalSemaphores[] = { m_RenderFinishedSemaphores[m_CurrentFrame] };

        submitInfo.signalSemaphoreCount =   1;
        submitInfo.pSignalSemaphores =      signalSemaphores;

        m_Device->ResetFences(
            { m_InFlightFences[m_CurrentFrame] }
        );

        if(m_Device->QueueSubmit(
            &submitInfo,
            m_InFlightFences[m_CurrentFrame]) == SE_FALSE)
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
        presentInfo.pImageIndices =     &frameIndex;
        presentInfo.pResults =          nullptr; // Optional

        result = m_Device->QueuePresent(
            &presentInfo
        );

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
        m_Device->WaitIdle();

        // m_Framebuffer->Cleanup(*m_LogicalDevice);
        // m_CommandPool->Cleanup(*m_LogicalDevice);
        // m_GraphicsPipeline->Cleanup(*m_LogicalDevice, *m_SwapChain);
        // m_RenderPass->Cleanup(*m_LogicalDevice);
        // m_SwapChain->Cleanup(*m_LogicalDevice);
        // m_DescriptorPool->Cleanup(*m_LogicalDevice);
        // m_Descriptor->Cleanup(*m_LogicalDevice);

        // m_Buffer->Cleanup(*m_LogicalDevice);
        // m_Buffer2->Cleanup(*m_LogicalDevice);
        // m_IndexBuffer->Cleanup(*m_LogicalDevice);

        // for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        // {
        //     vkDestroySemaphore(m_LogicalDevice->GetLogicalDevice(), m_RenderFinishedSemaphores[i], nullptr);
        //     vkDestroySemaphore(m_LogicalDevice->GetLogicalDevice(), m_ImageAvailableSemaphores[i], nullptr);
        //     vkDestroyFence(m_LogicalDevice->GetLogicalDevice(), m_InFlightFences[i], nullptr);
        // }

        // vkDestroyDevice(m_LogicalDevice->GetLogicalDevice(), nullptr);

        m_Validation->Cleanup(*this, mc_EnableValidationLayers);

        m_Surface->Cleanup(this);
        vkDestroyInstance(m_Instance, 0);
    }

    void Renderer::RecreateSwapChain()
    {
        // if(m_WindowMinimized)
        // {
        //     return;
        // }

        // vkDeviceWaitIdle(m_LogicalDevice->GetLogicalDevice());

        // m_Framebuffer->Cleanup(*m_LogicalDevice);
        // m_CommandPool->CleanupCommandBuffers(*m_LogicalDevice);
        // m_GraphicsPipeline->Cleanup(*m_LogicalDevice, *m_SwapChain);
        // m_RenderPass->Cleanup(*m_LogicalDevice);
        // m_SwapChain->Cleanup(*m_LogicalDevice);
        // m_DescriptorPool->Cleanup(*m_LogicalDevice);

        // m_SwapChain->Create(*m_PhysicalDevice, *m_LogicalDevice, *m_Surface);
        // m_RenderPass->Create(*m_LogicalDevice, *m_SwapChain);

        // m_ShaderStages.clear();
        // m_SomeShader->LoadShader(m_ShaderStages);
        // m_GraphicsPipeline->Create(*m_LogicalDevice, *m_SwapChain, *m_RenderPass, *m_Descriptor, m_ShaderStages);
        // m_SomeShader->Destroy();
        // m_Framebuffer->Create(*m_LogicalDevice, *m_SwapChain, *m_RenderPass);
        // m_GraphicsPipeline->CreateUniformBuffers(*m_PhysicalDevice, *m_LogicalDevice, *m_SwapChain, sizeof(UniformBufferObject));
        // m_DescriptorPool->Create(*m_LogicalDevice, *m_SwapChain, *m_Descriptor, *m_GraphicsPipeline);
        // m_CommandPool->CreateCommandBuffers(
        //     *m_LogicalDevice,
        //     *m_Framebuffer,
        //     *m_RenderPass,
        //     *m_SwapChain,
        //     *m_GraphicsPipeline,
        //     *m_DescriptorPool,
        //     m_VertexArray,
        //     m_IndexBuffer
        // );
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