#include "Editor/ImGUI/OpenGL_Context.h"

#include "RuntimeReflection/Reflection.h"

#include "Logger/Logger.h"

#include "Core/Core.h"

namespace SteelEngine {

    OpenGL_Context::OpenGL_Context()
    {

    }

    OpenGL_Context::~OpenGL_Context()
    {

    }

    void OpenGL_Context::Init(const IReflectionData* windowType, IWindow* window, const Graphics::RenderDevice* renderDevice)
    {
        m_Window = window;

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        io.ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_ViewportsEnable;

        m_Context = ImGui::GetCurrentContext();

        ImGui::StyleColorsDark();

        Variant cont = windowType->Invoke("GetContext", window);

        if(!cont.IsValid())
        {
            SE_ERROR("Context is not valid!");

            return;
        }

        ImGui_ImplSDL2_InitForOpenGL((SDL_Window*)window->GetWindow(), cont.Convert<void*>());
        ImGui_ImplOpenGL3_Init("#version 130");

        windowType->Invoke("MakeCurrent", window);
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

    void OpenGL_Context::ProcessEvent(const void* event)
    {
        ImGui_ImplSDL2_ProcessEvent((const SDL_Event*)event);
    }

}