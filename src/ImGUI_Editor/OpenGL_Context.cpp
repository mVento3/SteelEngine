#include "ImGUI_Editor/OpenGL_Context.h"

#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {

    OpenGL_Context::OpenGL_Context()
    {

    }

    OpenGL_Context::~OpenGL_Context()
    {

    }

    void OpenGL_Context::Init(IWindow* window, Graphics::IRenderer* renderer)
    {
        m_Window = window;

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        ImGui::StyleColorsDark();

        ImGui_ImplSDL2_InitForOpenGL((SDL_Window*)window->GetWindow(), Reflection::GetType("SteelEngine::OpenGL_Window")->Invoke("GetContext", window).Convert<void*>());
        ImGui_ImplOpenGL3_Init("#version 130");

        m_Context = ImGui::GetCurrentContext();

        Reflection::GetType("SteelEngine::OpenGL_Window")->Invoke("MakeCurrent", window);
        Reflection::GetType("SteelEngine::Graphics::OpenGL::Renderer")->Invoke("GlewInit", renderer);
    }

    void OpenGL_Context::Update()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame((SDL_Window*)m_Window->GetWindow());
    }

    void OpenGL_Context::UploadDrawData()
    {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void OpenGL_Context::operator()(const RecompiledEvent& event)
    {

    }

}