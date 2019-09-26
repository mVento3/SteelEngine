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
            SDL_WINDOW_VULKAN |
            SDL_WINDOW_RESIZABLE
        );

        if(!m_Window)
        {
            return SE_FALSE;
        }

        return SE_TRUE;
    }

    void VulkanWindow::Update()
    {
        while(SDL_PollEvent(&m_Event))
        {
            if(m_Event.type == SDL_QUIT)
            {
                Event::GlobalEvent::Broadcast(WindowCloseEvent{});
            }

            m_ProcessEventsCallback(&m_Event, this);
        }
    }

    void VulkanWindow::Close()
    {
        SDL_DestroyWindow(m_Window);

        m_Window = 0;
    }

    void VulkanWindow::WaitEvents()
    {
        SDL_WaitEvent(&m_Event);
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

    void VulkanWindow::GetWindowSize(Type::uint32& width, Type::uint32& height)
    {
        SDL_GetWindowSize(m_Window, (int*)&width, (int*)&height);
    }

    void* VulkanWindow::GetWindow() const
    {
        return m_Window;
    }

    Result VulkanWindow::GetVulkanInstanceExtensions(Type::uint32* enabledExtensionCount, const char** extensionNames)
    {
        if(!m_Window)
        {
            return SE_FALSE;
        }

        SDL_Vulkan_GetInstanceExtensions(m_Window, enabledExtensionCount, extensionNames);

        return SE_TRUE;
    }

    Result VulkanWindow::CreateVulkanSurface(void* instance, void** surface)
    {
        if(!m_Window)
        {
            return SE_FALSE;
        }

        SDL_Vulkan_CreateSurface(m_Window, (VkInstance)instance, (VkSurfaceKHR*)surface);

        if(!*surface)
        {
            return SE_FALSE;
        }

        return SE_TRUE;
    }

}