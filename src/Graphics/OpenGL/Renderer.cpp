#include "Graphics/OpenGL/Renderer.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    Renderer::Renderer(IWindow* window) :
        m_Window(window)
    {

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

        return SE_TRUE;
    }

    void Renderer::Update()
    {
        
    }

    void Renderer::Cleanup()
    {

    }

    Result Renderer::GlewInit()
    {
        glewExperimental = GL_TRUE;

        if(glewInit() != GL_TRUE)
        {
            return SE_FALSE;
        }

        return SE_TRUE;
    }

    void Renderer::PreRender()
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.2f, 0.2f, 0.2f, 1);
    }

    void Renderer::Render()
    {

    }

    void Renderer::PostRender()
    {
        m_Window->SwapBuffers();
    }

    void Renderer::operator()(const RecompiledEvent& event)
    {

    }

}}}