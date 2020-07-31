#include "Utils/Window.h"

#include "Window/VulkanWindow.h"
#include "Window/OpenGL_Window.h"

#include "RuntimeReflection/Reflection.h"

#include "Utils/Graphics/RenderContext.h"

namespace SteelEngine {

    Window::Window()
    {

    }

    Window::~Window()
    {

    }

    IWindow* Window::Create(WindowAPI api)
    {
        switch(api)
        {
        case WindowAPI::SDL:
        {
            IWindow* window = 0;

            if(Utils::RenderContext::GetCurrentAPI() == Graphics::IRenderer::API::OPENGL)
            {
                window = (IWindow*)Reflection::CreateInstance<OpenGL_Window>();
            }
            else if(Utils::RenderContext::GetCurrentAPI() == Graphics::IRenderer::API::VULKAN)
            {
                window = (IWindow*)Reflection::CreateInstance<VulkanWindow>();
            }

            return window;
        }
        default:
            return 0;
        }
    }

}