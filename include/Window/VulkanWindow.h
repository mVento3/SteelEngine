#pragma once

#include "Window/IWindow.h"

#include "SDL.h"
#include "SDL_vulkan.h"

#include "RuntimeReflection/Macro.h"

#include "HotReloader/IRuntimeObject.h"

#include "Input/Events/ChangeMousePositionEvent.h"

#include "functional"

namespace SteelEngine {

    SE_CLASS(
        SteelEngine::Reflection::ReflectionAttribute::NO_SERIALIZE
    )
    class VulkanWindow : public IWindow
    {
    public:
        typedef void(*ProcessEventsCallback)(void*);

    private:
        std::string m_Title;
        Type::uint32 m_Width;
        Type::uint32 m_Height;

        SDL_Window* m_Window;
        SDL_Event m_Event;
        
    public:
        VulkanWindow();
        ~VulkanWindow();

        std::function<void(void*, IWindow*)> m_ProcessEventsCallback;

        Result Create() override;
        void Update() override;
        void Close() override;
        void WaitEvents() override;

        void SetTitle(const std::string& title) override;
        void SetWidth(const Type::uint32& width) override;
        void SetHeight(const Type::uint32& height) override;

        void GetWindowSize(Type::uint32& width, Type::uint32& height) override;
        void* GetWindow() const override;

        Result GetVulkanInstanceExtensions(Type::uint32* enabledExtensionCount, const char** extensionNames) override;
        Result CreateVulkanSurface(void* instance, void** surface) override;

        SE_METHOD()
        inline void SetProcessEventsCallback(std::function<void(void*, IWindow*)> callback)
        {
            m_ProcessEventsCallback = callback;
        }

        void operator()(const ChangeMousePositionEvent& event);
    };

}