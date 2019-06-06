#include "Window/VulkanWindow.h"

#include "Event/GlobalEvent.h"

namespace SteelEngine {

    VulkanWindow::VulkanWindow()
    {

    }

    VulkanWindow::~VulkanWindow()
    {

    }

    Result VulkanWindow::Create()
    {
        if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            return SE_FALSE;
        }

        m_Window = SDL_CreateWindow(
            m_Title.c_str(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            m_Width,
            m_Height,
            SDL_WINDOW_VULKAN
        );

        if(!m_Window)
        {
            return SE_FALSE;
        }

        

        return SE_TRUE;
    }

    void VulkanWindow::Update()
    {
        SDL_Event event;

        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT ||
                event.key.keysym.scancode == SDL_SCANCODE_ESCAPE ||
                (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(m_Window)))
            {
                Event::GlobalEvent::Broadcast(WindowCloseEvent{});
            }
        }
    }

    void VulkanWindow::Close()
    {
        SDL_DestroyWindow(m_Window);

        m_Window = 0;
    }

    void VulkanWindow::SetTitle(const std::string& title)
    {
        m_Title = title;
    }

    void VulkanWindow::SetWidth(const Type::uint32& width)
    {
        m_Width = width;
    }

    void VulkanWindow::SetHeight(const Type::uint32& height)
    {
        m_Height = height;
    }

}