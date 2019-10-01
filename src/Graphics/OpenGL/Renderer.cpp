#include "Graphics/OpenGL/Renderer.h"

#include "SDL_events.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    Renderer::Renderer(IWindow* window) :
        m_Window(window)
    {
        m_QuadShader = new Shader("D:/Projects/C++/SteelEngine/bin/Resources/Shaders/OpenGL/quadShader");
        m_QuadMesh = new QuadMesh();
        m_Renderbuffer = new Renderbuffer();

        m_Mesh = new Mesh("D:/Projects/C++/SteelEngine/bin/Resources/Models/test.obj");
        m_Mesh2 = new Mesh("D:/Projects/C++/SteelEngine/bin/Resources/Models/cube.obj");
        m_Texture = new Texture("D:/Projects/C++/SteelEngine/bin/Resources/Textures/bricks2.jpg");
        m_NormalMapTexture = new Texture("D:/Projects/C++/SteelEngine/bin/Resources/Textures/bricks2_normal.jpg");
        m_DispMapTexture = new Texture("D:/Projects/C++/SteelEngine/bin/Resources/Textures/bricks2_disp.jpg");

        m_PositionTexture = new Texture(1600, 900, GL_RGB16F, GL_RGB, GL_UNSIGNED_BYTE, GL_NEAREST, GL_NEAREST);
        m_NormalTexture = new Texture(1600, 900, GL_RGB16F, GL_RGB, GL_UNSIGNED_BYTE, GL_NEAREST, GL_NEAREST);
        m_AlbedoTexture = new Texture(1600, 900, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, GL_NEAREST, GL_NEAREST);

        std::vector<Framebuffer::Attachment*> attachments =
        {
            new Framebuffer::Attachment(m_PositionTexture, GL_COLOR_ATTACHMENT0),
            new Framebuffer::Attachment(m_NormalTexture, GL_COLOR_ATTACHMENT1),
            new Framebuffer::Attachment(m_AlbedoTexture, GL_COLOR_ATTACHMENT2)
        };

        m_G_Shader = new Shader("D:/Projects/C++/SteelEngine/bin/Resources/Shaders/OpenGL/gBuffer");
        m_G_Buffer = new Framebuffer(attachments);

        m_Counter = 0;
        m_Camera = new Camera(Transform(glm::vec3(0, 0, -10)));

        m_Trans.SetScale(glm::vec3(100, 100, 100));
        m_Trans2.SetPosition(glm::vec3(0, 10, 0));
    }

    Renderer::~Renderer()
    {

    }

    Result Renderer::Init()
    {
        glewExperimental = GL_TRUE;

        GLenum err = glewInit();

        if(GLEW_OK != err)
        {
            printf("Error: %s\n", glewGetErrorString(err));

            return SE_FALSE;
        }

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glEnable(GL_DEPTH_CLAMP);

    // Models in scene -------------------
        m_Mesh->Setup();
        m_Texture->Setup();
        m_Mesh2->Setup();
        m_NormalMapTexture->Setup();
        m_DispMapTexture->Setup();
    //

    // GBuffer shader ---------------------------
        m_G_Shader->Init();
        m_G_Shader->Bind();
        m_G_Shader->SetInt("albedoMap", 0);
        m_G_Shader->SetInt("normalMap", 1);
        m_G_Shader->SetInt("dispMap", 2);
    //

    // Quad shader ------------------------
        m_QuadShader->Init();
        m_QuadShader->Bind();
        m_QuadShader->SetInt("gPosition", 0);
        m_QuadShader->SetInt("gNormal", 1);
        m_QuadShader->SetInt("gAlbedoSpec", 2);
        m_QuadMesh->Setup();
    //

    // GBuffer framebuffer ------------------------
        m_PositionTexture->Setup();
        m_NormalTexture->Setup();
        m_AlbedoTexture->Setup();
        m_G_Buffer->Setup();

        unsigned int attachments[3] =
        {
            GL_COLOR_ATTACHMENT0,
            GL_COLOR_ATTACHMENT1,
            GL_COLOR_ATTACHMENT2
        };

        glDrawBuffers(3, attachments);
    //

        m_Renderbuffer->Setup();

        Event::GlobalEvent::Add<KeyDownEvent>(this);
        Event::GlobalEvent::Add<KeyUpEvent>(this);
        Event::GlobalEvent::Add<MouseMotionEvent>(this);

        return SE_TRUE;
    }

    void Renderer::Update()
    {
        m_Counter += 0.01f;

        Transform& camTrans = m_Camera->GetTransform();

        if(m_Keys[SDL_SCANCODE_W])
        {
            camTrans.SetPosition(camTrans.GetPosition() + camTrans.GetRotation().GetForward() * 0.5f);
        }

        if(m_Keys[SDL_SCANCODE_S])
        {
            camTrans.SetPosition(camTrans.GetPosition() + camTrans.GetRotation().GetBackward() * 0.5f);
        }

        if(m_Keys[SDL_SCANCODE_A])
        {
            camTrans.SetPosition(camTrans.GetPosition() + camTrans.GetRotation().GetRight() * 0.5f);
        }

        if(m_Keys[SDL_SCANCODE_D])
        {
            camTrans.SetPosition(camTrans.GetPosition() + camTrans.GetRotation().GetLeft() * 0.5f);
        }

        if(m_RotateCamera)
        {
            Event::GlobalEvent::Broadcast(ChangeMousePositionEvent{ 1600 / 2, 900 / 2 });
        }
    }

    void Renderer::Cleanup()
    {

    }

    void Renderer::PreRender()
    {
        Type::uint32 w;
        Type::uint32 h;

        m_Window->GetWindowSize(w, h);

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glViewport(0, 0, w, h);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.2f, 0.2f, 1);
    }

    void Renderer::Render()
    {
    // Objects data
        m_G_Buffer->Bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_Renderbuffer->Bind();
        m_G_Shader->Bind();
        m_Texture->Bind(GL_TEXTURE0);
        m_NormalMapTexture->Bind(GL_TEXTURE1);
        m_DispMapTexture->Bind(GL_TEXTURE2);

        m_G_Shader->SetVec3("ambientLight", glm::vec3(0.1f, 0.1f, 0.1f));
        m_G_Shader->SetVec3("baseColor", glm::vec3(1, 1, 1));

        m_G_Shader->SetVec3("eyePosition", m_Camera->GetTransform().GetPosition());

        m_G_Shader->SetFloat("specularIntensity", 2.f);
        m_G_Shader->SetFloat("specularPower", 32.f);

        float dispMapScale = 0.05f;
        float dispMapOffset = -1.f;
        float dispMapBias = dispMapScale / 2.0f;

        m_G_Shader->SetFloat("dispMapScale", dispMapScale);
        m_G_Shader->SetFloat("dispMapOffset", -dispMapBias + dispMapBias * dispMapOffset);

        m_G_Shader->SetVec3("directionalLight.base.color", glm::vec3(1, 1, 1));
        m_G_Shader->SetFloat("directionalLight.base.intensity", 1.f);
        m_G_Shader->SetVec3("directionalLight.direction", glm::vec3(1, 1, 1));

        m_G_Shader->Update(m_Trans, *m_Camera);
        m_Mesh->Draw();

        m_G_Shader->Update(m_Trans2, *m_Camera);
        m_Mesh2->Draw();

    // Render quad
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        m_QuadShader->Bind();

        m_PositionTexture->Bind(GL_TEXTURE0);
        m_NormalTexture->Bind(GL_TEXTURE1);
        m_AlbedoTexture->Bind(GL_TEXTURE2);

        m_QuadMesh->Draw();
    }

    void Renderer::PostRender()
    {
        m_Window->SwapBuffers();
    }

    void Renderer::operator()(const KeyDownEvent& event)
    {
        m_Keys[event.m_KeyCode] = true;

        if(event.m_KeyCode == SDL_SCANCODE_K)
        {
            m_RotateCamera = !m_RotateCamera;
        }
    }

    void Renderer::operator()(const KeyUpEvent& event)
    {
        m_Keys[event.m_KeyCode] = false;
    }

    void Renderer::operator()(const MouseMotionEvent& event)
    {
        glm::vec2 deltaPos = glm::vec2(event.m_X, event.m_Y) - glm::vec2(1600 / 2, 900 / 2);

        if(m_RotateCamera && (deltaPos.x != 0 || deltaPos.y != 0))
        {
            Transform& camTrans = m_Camera->GetTransform();

            camTrans.SetRotation(glm::normalize(glm::angleAxis(deltaPos.x * -0.001f, glm::vec3(0, 1, 0)) * camTrans.GetRotation()));
            camTrans.SetRotation(glm::normalize(glm::angleAxis(deltaPos.y * 0.001f, camTrans.GetRotation().GetRight()) * camTrans.GetRotation()));
        }
    }

    void Renderer::operator()(const RecompiledEvent& event)
    {

    }

}}}