#pragma once

// #include "HotReloader/IRuntimeObject.h"

#include "Core/Result.h"

#include "functional"

namespace SteelEngine {

    struct IWindow
    {
        struct WindowCloseEvent
        {

        };

        virtual Result Create()
        {
            return SE_NOT_IMPLEMENTED;
        }

        virtual void Update()
        {
            
        }

        virtual void Close()
        {
            
        }

        virtual void WaitEvents()
        {

        }

        virtual void SwapBuffers()
        {

        }

        virtual void SetTitle(const std::string& title)
        {

        }

        virtual void SetWidth(const Type::uint32& width)
        {

        }

        virtual void SetHeight(const Type::uint32& height)
        {

        }

        virtual void GetWindowSize(Type::uint32& width, Type::uint32& height)
        {

        }

        virtual void* GetWindow() const
        {
            return 0;
        }

        virtual void SetProcessEventsCallback(std::function<void(void*, IWindow*)> callback) = 0;

    // Vulkan
        virtual Result GetVulkanInstanceExtensions(Type::uint32* enabledExtensionCount, const char** extensionNames)
        {
            return SE_NOT_IMPLEMENTED;
        }

        virtual Result CreateVulkanSurface(void* instance, void** surface)
        {
            return SE_NOT_IMPLEMENTED;
        }
    };

}