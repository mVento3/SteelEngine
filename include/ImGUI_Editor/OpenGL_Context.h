#pragma once

#include "ImGUI_Editor/IContext.h"

#include "imgui/imgui.h"
#include "imgui/examples/imgui_impl_sdl.h"
#include "imgui/examples/imgui_impl_opengl3.h"

#include "RuntimeReflection/Macro.h"

#include "ImGUI_Editor/OpenGL_Context.Generated.h"

namespace SteelEngine {

    SE_CLASS()
    class OpenGL_Context : public IContext
    {
        GENERATED_BODY
    private:
        ImGuiContext* m_Context;
        IWindow* m_Window;

    public:
        OpenGL_Context();
        ~OpenGL_Context();

        void Init(IWindow* window, Graphics::IRenderer* renderer) override;
        void Update() override;
        void UploadDrawData() override;

        SE_METHOD()
        inline void* GetContext() { return m_Context; }
    };

}