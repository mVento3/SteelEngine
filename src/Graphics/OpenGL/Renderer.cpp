#include "Graphics/OpenGL/Renderer.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    Renderer::Renderer(IWindow* window) :
        m_Window(window)
    {
        m_Shader = new Shader("D:/Projects/C++/SteelEngine/bin/Resources/Shaders/OpenGL/testShader");
        m_Mesh = new Mesh(
            {
                Vertex(glm::vec3(-0.5f, -0.5f, 0)),
                Vertex(glm::vec3(0, 0.5f, 0)),
                Vertex(glm::vec3(0.5f, -0.5f, 0))
            }
        );
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

        m_Shader->Init();
        m_Mesh->Setup();

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
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.2f, 0.2f, 0.2f, 1);
    }

    void Renderer::Render()
    {
        m_Shader->Bind();
        m_Mesh->Draw();
    }

    void Renderer::PostRender()
    {
        m_Window->SwapBuffers();
    }

    void Renderer::operator()(const RecompiledEvent& event)
    {

    }

}}}