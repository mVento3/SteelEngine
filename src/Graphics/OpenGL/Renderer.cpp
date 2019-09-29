#include "Graphics/OpenGL/Renderer.h"

#include "SDL_events.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    Renderer::Renderer(IWindow* window) :
        m_Window(window)
    {
        m_Shader = new Shader("D:/Projects/C++/SteelEngine/bin/Resources/Shaders/OpenGL/testShader2");
        m_Mesh = new Mesh("D:/Projects/C++/SteelEngine/bin/Resources/Models/test.obj");
        m_Texture = new Texture("D:/Projects/C++/SteelEngine/bin/Resources/Textures/bricks2.jpg");
        m_NormalMapTexture = new Texture("D:/Projects/C++/SteelEngine/bin/Resources/Textures/bricks2_normal.jpg");

        m_PositionTexture = new Texture(1600, 900, GL_RGB16F, GL_RGB, GL_FLOAT, GL_NEAREST, GL_NEAREST);
        m_NormalTexture = new Texture(1600, 900, GL_RGB16F, GL_RGB, GL_FLOAT, GL_NEAREST, GL_NEAREST);
        m_AlbedoTexture = new Texture(1600, 900, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, GL_NEAREST, GL_NEAREST);

        std::vector<Framebuffer::Attachment*> attachments =
        {
            new Framebuffer::Attachment(m_PositionTexture, GL_COLOR_ATTACHMENT0),
            new Framebuffer::Attachment(m_NormalTexture, GL_COLOR_ATTACHMENT1),
            new Framebuffer::Attachment(m_AlbedoTexture, GL_COLOR_ATTACHMENT2)
        };

        m_G_Shader = new Shader("D:/Projects/C++/SteelEngine/bin/Resources/Shaders/OpenGL/gBuffer");
        m_G_Buffer = new Framebuffer(attachments);
        m_QuadMesh = new QuadMesh();
        m_Renderbuffer = new Renderbuffer();

        m_Counter = 0;
        m_Camera = new Camera(Transform(glm::vec3(0, 0, -10)));

        m_Trans.SetScale(glm::vec3(10, 10, 10));
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

        m_Shader->Init();
        m_Mesh->Setup();
        m_Texture->Setup();
        m_NormalMapTexture->Setup();

        m_G_Shader->Init();
        m_G_Shader->Bind();
        m_G_Shader->SetInt("albedoMap", 0);
        m_G_Shader->SetInt("normalMap", 1);

        m_Shader->Bind();
        m_Shader->SetInt("gPosition", 0);
        m_Shader->SetInt("gNormal", 1);
        m_Shader->SetInt("gAlbedoSpec", 2);

        m_PositionTexture->Setup();
        m_NormalTexture->Setup();
        m_AlbedoTexture->Setup();
        m_G_Buffer->Setup();
        m_QuadMesh->Setup();

        unsigned int attachments[3] =
        {
            GL_COLOR_ATTACHMENT0,
            GL_COLOR_ATTACHMENT1,
            GL_COLOR_ATTACHMENT2
        };

        glDrawBuffers(3, attachments);

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
            camTrans.SetPosition(camTrans.GetPosition() + camTrans.GetForwardVector() * 0.5f);
        }

        if(m_Keys[SDL_SCANCODE_S])
        {
            camTrans.SetPosition(camTrans.GetPosition() + camTrans.GetBackwardVector() * 0.5f);
        }

        if(m_Keys[SDL_SCANCODE_A])
        {
            camTrans.SetPosition(camTrans.GetPosition() + camTrans.GetRightVector() * 0.5f);
        }

        if(m_Keys[SDL_SCANCODE_D])
        {
            camTrans.SetPosition(camTrans.GetPosition() + camTrans.GetLeftVector() * 0.5f);
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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.2f, 0.2f, 1);
    }

    void Renderer::Render()
    {
        m_G_Buffer->Bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_G_Shader->Bind();
        m_Texture->Bind(GL_TEXTURE0);
        m_NormalMapTexture->Bind(GL_TEXTURE1);
        m_G_Shader->Update(m_Trans, *m_Camera);

        m_Mesh->Draw();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_Shader->Bind();
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
        int deltaX = (1600 / 2) - event.m_X;
        int deltaY = (900 / 2) - event.m_Y;

        if(m_RotateCamera)
        {
            Transform& camTrans = m_Camera->GetTransform();

            camTrans.SetRotation(glm::quat(glm::rotate(deltaX * 0.001f, glm::vec3(0, 1, 0))) * camTrans.GetRotation());
            camTrans.SetRotation(glm::quat(glm::rotate(deltaY * -0.001f, camTrans.GetRotation() * glm::vec3(1, 0, 0))) * camTrans.GetRotation());
        }
    }

    void Renderer::operator()(const RecompiledEvent& event)
    {

    }

}}}