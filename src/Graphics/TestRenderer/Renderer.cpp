#include "Graphics/TestRenderer/Renderer.h"

#include "Utils/Graphics/Texture.h"
#include "Utils/Graphics/Framebuffer.h"
#include "Utils/Graphics/Shader.h"
#include "Utils/Graphics/Mesh.h"

#include "Utils/Graphics/RenderContext.h"

namespace SteelEngine { namespace Graphics { namespace TestRenderer {

    using Texture = SteelEngine::Utils::Graphics::Texture;
    using Framebuffer = SteelEngine::Utils::Graphics::Framebuffer;
    using Shader = SteelEngine::Utils::Graphics::Shader;
    using Mesh = SteelEngine::Utils::Graphics::Mesh;

    Renderer::Renderer()
    {

    }

    Renderer::~Renderer()
    {

    }

    Result Renderer::Init()
    {
        m_AlbedoTexture = Texture::Create(InternalFormat::RGBA, Format::RGBA, DataType::UNSIGNED_BYTE, Filter::NEAREST, false);
    
        m_GBuffer = Framebuffer::Create(1920, 1080,
        {
            FramebufferAttachment(m_AlbedoTexture, FramebufferAttachment::Attachment::COLOR_ATTACHMENT0)
        });

        m_GBuffer->Setup(m_RenderContext->GetRenderDevice());

        m_QuadShader = Shader::Create("D:/Projects/C++/SteelEngine/bin/Resources/Shaders/TestRenderer/gBuffer");

        m_QuadShader->Setup(m_RenderContext->GetRenderDevice());

        glm::vec3 vertices[] =
        {
            glm::vec3(-0.5, -0.5, 0),
            glm::vec3(0, 0.5, 0),
            glm::vec3(0.5, -0.5, 0)
        };

        uint32_t indices[] = { 0, 1, 2 };

        // m_Vertices =
        // {
        //     Vertex(glm::vec3(-1, 1, 0), glm::vec2(0, 1), glm::vec3(), glm::vec3()),
        //     Vertex(glm::vec3(-1, -1, 0), glm::vec2(0, 0), glm::vec3(), glm::vec3()),
        //     Vertex(glm::vec3(1, 1, 0), glm::vec2(1, 1), glm::vec3(), glm::vec3()),
        //     Vertex(glm::vec3(1, -1, 0), glm::vec2(1, 0), glm::vec3(), glm::vec3())
        // };

        m_QuadMesh = Mesh::Create();

        m_RenderContext->GetRenderDevice()->Begin(m_QuadMesh, 2);
        m_RenderContext->GetRenderDevice()->PassVBO(m_QuadMesh, 0, vertices, sizeof(vertices) / sizeof(vertices[0]), sizeof(vertices[0]), 3);
        m_RenderContext->GetRenderDevice()->PassIBO(m_QuadMesh, 1, indices, sizeof(indices) / sizeof(indices[0]), sizeof(indices[0]));
        m_RenderContext->GetRenderDevice()->End(m_QuadMesh);

        return SE_TRUE;  
    }

    void Renderer::Update()
    {

    }

    void Renderer::Cleanup()
    {

    }

    void Renderer::PreRender()
    {

    }

    void Renderer::Render(entt::registry& scene)
    {
        // m_RenderDevice->Bind(m_GBuffer);

        m_RenderContext->GetRenderDevice()->ClearColor(0.2f, 0.2f, 0.2f, 1);
        // m_RenderDevice->Clear(ClearMask::COLOR_BUFFER_BIT | ClearMask::DEPTH_BUFFER_BIT);

        // m_RenderDevice->Bind(m_QuadShader);
        // m_RenderDevice->Draw(m_QuadMesh);

        // m_RenderDevice->BindFramebuffer(0);

        m_RenderContext->GetRenderDevice()->DrawTriangles(0);
    }

    void Renderer::PostRender()
    {

    }

    void Renderer::OnEvent(Event::Naive* event)
    {

    }

    void* Renderer::GetFinalTexture()
    {
        RenderDevice* rd = m_RenderContext->GetRenderDevice();
        void** l = rd->GetDescriptor();
        void* l2 = *l;

        return l2;
    }

    void Renderer::SetRenderTarget(IWindow* window)
    {
        m_Window = window;
    }

    void Renderer::SetSceneManager(ISceneManager* scene)
    {

    }

    void Renderer::SetRenderContext(SteelEngine::Utils::RenderContext* context)
    {
        m_RenderContext = context;
    }

}}}