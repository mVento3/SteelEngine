#include "Graphics/OpenGL/Renderer.h"

#include "SDL_events.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    Renderer::Renderer(IWindow* window) :
        m_Window(window)
    {
        m_Shader = new Shader("D:/Projects/C++/SteelEngine/bin/Resources/Shaders/OpenGL/testShader");
        // m_Mesh = new Mesh(
        //     {
        //         Vertex(glm::vec3(-0.5f, -0.5f, 0), glm::vec2(0, 0)),
        //         Vertex(glm::vec3(0, 0.5f, 0), glm::vec2(0.5f, 1)),
        //         Vertex(glm::vec3(0.5f, -0.5f, 0), glm::vec2(1, 0))
        //     }
        // );
        m_Mesh = new Mesh("D:/Projects/C++/SteelEngine/bin/Resources/Models/test.obj");
        m_Texture = new Texture("D:/Projects/C++/SteelEngine/bin/Resources/Textures/test.jpg");

        m_Counter = 0;

        m_Camera = new Camera(Transform(glm::vec3(0, 0, -10)));
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

        Event::GlobalEvent::Add<KeyDownEvent>(this);
        Event::GlobalEvent::Add<KeyUpEvent>(this);
        Event::GlobalEvent::Add<MouseMotionEvent>(this);

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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.2f, 0.2f, 1);
    }

    void Renderer::Render()
    {
        m_Shader->Bind();
        m_Shader->Update(m_Trans, *m_Camera);
        m_Texture->Bind(0);
        m_Mesh->Draw();

        m_Counter += 0.01f;

        m_Trans.SetRotation(glm::rotate(m_Counter, glm::vec3(1, 0, 0)));
        // m_Camera->GetTransform().SetRotation(glm::rotate(m_Counter, glm::vec3(0, 1, 0)));

        Transform& camTrans = m_Camera->GetTransform();

        if(m_Keys[SDLK_w])
        {
            camTrans.SetPosition(camTrans.GetPosition() + (camTrans.GetRotation() * glm::vec3(0, 0, 1) * 0.5f));
        }

        if(m_Keys[SDLK_s])
        {
            camTrans.SetPosition(camTrans.GetPosition() + (camTrans.GetRotation() * glm::vec3(0, 0, -1) * 0.5f));
        }

        if(m_Keys[SDLK_a])
        {
            camTrans.SetPosition(camTrans.GetPosition() + (camTrans.GetRotation() * glm::vec3(1, 0, 0) * 0.5f));
        }

        if(m_Keys[SDLK_d])
        {
            camTrans.SetPosition(camTrans.GetPosition() + (camTrans.GetRotation() * glm::vec3(-1, 0, 0) * 0.5f));
        }

        if(m_RotateCamera)
        {
            Event::GlobalEvent::Broadcast(ChangeMousePositionEvent{ 1600 / 2, 900 / 2 });
        }
    }

    void Renderer::PostRender()
    {
        m_Window->SwapBuffers();
    }

    void Renderer::operator()(const KeyDownEvent& event)
    {
        m_Keys[event.m_KeyCode] = true;

        if(event.m_KeyCode == SDLK_k)
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