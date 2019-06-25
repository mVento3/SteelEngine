#pragma once

#include "Window/IWindow.h"

#include "SDL.h"
#include "SDL_vulkan.h"

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/ReflectionAttributes.h"

#include "RuntimeCompiler/IRuntimeObject.h"

#include "functional"

namespace SteelEngine {

    SE_CLASS(SteelEngine::ReflectionAttribute::SE_NO_SERIALIZE)
    class VulkanWindow : public Interface::IWindow
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

        std::function<void(void*, Interface::IWindow*)> m_ProcessEventsCallback;

        Result Create() override;
        void Update() override;
        void Close() override;
        void WaitEvents() override;

        void SetTitle(const std::string& title) override;
        void SetWidth(const Type::uint32& width) override;
        void SetHeight(const Type::uint32& height) override;

        void GetWindowSize(Type::uint32* width, Type::uint32* height) override;

        Result GetVulkanInstanceExtensions(Type::uint32* enabledExtensionCount, const char** extensionNames) override;
        Result CreateVulkanSurface(void* instance, void** surface) override;

        SE_METHOD()
        void SetProcessEventsCallback(std::function<void(void*, Interface::IWindow*)> callback);
    };

}