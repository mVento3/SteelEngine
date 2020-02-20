#pragma once

#include "Graphics/IContext.h"

#include "imgui/imgui.h"
#include "imgui/examples/imgui_impl_sdl.h"
#include "imgui/examples/imgui_impl_opengl3.h"

#include "RuntimeReflection/Macro.h"

#include "ImGUI_Editor/OpenGL_Context.Generated.h"

namespace SteelEngine {

    SE_CLASS(
        Reflection::ReflectionAttribute::HOT_RELOAD
    )
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
        void ProcessEvent(const void* event) override;

        SE_METHOD()
        inline void* GetContext() { return m_Context; }

        SE_METHOD()
        inline void MakeCurrent() { ImGui::SetCurrentContext(m_Context); }
    };

}