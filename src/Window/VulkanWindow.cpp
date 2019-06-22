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
        SDL_Event event;

        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                Event::GlobalEvent::Broadcast(WindowCloseEvent{});
            }

            m_ProcessEventsCallback(&event);
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

    void VulkanWindow::SetProcessEventsCallback(std::function<void(void*)> callback)
    {
        m_ProcessEventsCallback = callback;
    }

}