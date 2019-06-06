#pragma once

#include "Window/IWindow.h"

#include "SDL.h"

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/ReflectionAttributes.h"

#include "RuntimeCompiler/IRuntimeObject.h"

namespace SteelEngine {

    SE_CLASS(SteelEngine::ReflectionAttribute::SE_NO_SERIALIZE)
    class VulkanWindow : public Interface::IWindow
    {
    private:
        std::string m_Title;
        Type::uint32 m_Width;
        Type::uint32 m_Height;

        SDL_Window* m_Window;
        
    public:
        VulkanWindow();
        ~VulkanWindow();

        Result Create() override;
        void Update() override;
        void Close() override;

        void SetTitle(const std::string& title) override;
        void SetWidth(const Type::uint32& width) override;
        void SetHeight(const Type::uint32& height) override;
    };

}