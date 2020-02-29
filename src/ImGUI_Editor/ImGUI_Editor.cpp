#include "ImGUI_Editor/ImGUI_Editor.h"

#include "imgui/imgui_internal.h"

#include "algorithm"

#include "RuntimeReflection/Reflection.h"

#include "Core/Core.h"

#include "Profiler/ScopeTimer.h"

#undef min
#undef max

#define SE_EDITOR_NOT_IMPLEMENTED_ADDNOTATION \
    if(ImGui::IsItemHovered()) \
    { \
        ImGui::BeginTooltip(); \
        { \
            ImGui::Text("Not implemented yet!"); \
        } \
        ImGui::EndTooltip(); \
    }

namespace SteelEngine { namespace Editor { namespace ImGUI {

    void ImGUI_Editor::LoadProject()
    {

    }

    void ImGUI_Editor::CreateNewProject()
    {

    }

    ImGUI_Editor::ImGUI_Editor()
    {
        m_CurrentScene = SceneType::START_MENU_SCENE;
    }

    ImGUI_Editor::~ImGUI_Editor()
    {

    }

    Result ImGUI_Editor::Init(Graphics::IRenderer* renderer, IContext* context)
    {
        m_API_Context = context;
        m_Context = (ImGuiContext*)Reflection::GetType("SteelEngine::OpenGL_Context")->Invoke("GetContext", context).Convert<void*>();
        m_NaiveManager = Reflection::GetType("SteelEngine::Core")->GetMetaData(SteelEngine::Core::GlobalSystems::EVENT_MANAGER)->Convert<IEventManager*>();

        ImGui::SetCurrentContext(m_Context);

        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_DockingEnable;

        ImGuiStyle &st = ImGui::GetStyle();

        st.FrameBorderSize = 1.0f;
        st.FramePadding = ImVec2(4.0f,2.0f);
        st.ItemSpacing = ImVec2(8.0f,2.0f);
        st.WindowBorderSize = 1.0f;
        st.TabBorderSize = 1.0f;
        st.WindowRounding = 1.0f;
        st.ChildRounding = 1.0f;
        st.FrameRounding = 1.0f;
        st.ScrollbarRounding = 1.0f;
        st.GrabRounding = 1.0f;
        st.TabRounding = 1.0f;

        ImVec4* colors = ImGui::GetStyle().Colors;

        colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 0.95f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.12f, 0.12f, 1.00f);
        colors[ImGuiCol_ChildBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.94f);
        colors[ImGuiCol_Border] = ImVec4(0.53f, 0.53f, 0.53f, 0.46f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.85f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.22f, 0.22f, 0.22f, 0.40f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.16f, 0.16f, 0.16f, 0.53f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.48f, 0.48f, 0.48f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.79f, 0.79f, 0.79f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.48f, 0.47f, 0.47f, 0.91f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.55f, 0.55f, 0.62f);
        colors[ImGuiCol_Button] = ImVec4(0.50f, 0.50f, 0.50f, 0.63f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.67f, 0.67f, 0.68f, 0.63f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.26f, 0.26f, 0.26f, 0.63f);
        colors[ImGuiCol_Header] = ImVec4(0.54f, 0.54f, 0.54f, 0.58f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.64f, 0.65f, 0.65f, 0.80f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.25f, 0.25f, 0.25f, 0.80f);
        colors[ImGuiCol_Separator] = ImVec4(0.58f, 0.58f, 0.58f, 0.50f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.81f, 0.81f, 0.81f, 0.64f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.81f, 0.81f, 0.81f, 0.64f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.87f, 0.87f, 0.87f, 0.53f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.87f, 0.87f, 0.87f, 0.74f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.87f, 0.87f, 0.87f, 0.74f);
        colors[ImGuiCol_Tab] = ImVec4(0.01f, 0.01f, 0.01f, 0.86f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
        colors[ImGuiCol_TabActive] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.02f, 0.02f, 0.02f, 1.00f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
        colors[ImGuiCol_DockingPreview] = ImVec4(0.38f, 0.48f, 0.60f, 1.00f);
        colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.68f, 0.68f, 0.68f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.77f, 0.33f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.87f, 0.55f, 0.08f, 1.00f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.47f, 0.60f, 0.76f, 0.47f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(0.58f, 0.58f, 0.58f, 0.90f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

        m_Types = Reflection::GetTypes();

        for(Type::uint32 i = 0; i < Reflection::GetTypesSize(); i++)
        {
            const IReflectionData* type = m_Types[i];
            std::vector<IReflectionInheritance*> inhs = type->GetInheritances();

            for(Type::uint32 i = 0; i < inhs.size(); i++)
            {
                if(inhs[i]->GetTypeID() == typeid(EditorComponents::ImGUI::UserInterface).hash_code() && type->GetMetaData(ReflectionAttributes::EDITOR_WINDOW)->Convert<bool>())
                {
                    void** windowPtr = type->Create_();

                    HotReloader::InheritanceTrackKeeper* swapper = new HotReloader::InheritanceTrackKeeper(type, windowPtr);
                    EditorComponents::ImGUI::UserInterface* window = swapper->Get<EditorComponents::ImGUI::UserInterface>();

                    strcpy(window->m_Title, type->GetTypeName());
                    window->m_Context = m_Context;
                    window->m_Editor = (ImGUI_Editor**)&m_Object;

                    if(type->GetMetaData(ReflectionAttributes::EDITOR_WINDOW)->Convert<bool>())
                    {
                        if(type->GetMetaData(ReflectionAttributes::SCENE_TYPE)->Convert<SceneType>() & SceneType::EDITOR_SCENE)
                        {
                            m_MainEditorWindows.push_back(swapper);
                        }

                        if(type->GetMetaData(ReflectionAttributes::SCENE_TYPE)->Convert<SceneType>() & SceneType::START_MENU_SCENE)
                        {
                            m_StartMenuWindows.push_back(swapper);
                        }
                    }

                    window->Init();
                }
            }
        }

        IEditor::Init(*m_VirtualProjectVisualizer, context);

        Event::GlobalEvent::Add<ChangeSceneEvent>(this);

        int width, height, channels;

        // data = stbi_load("D:/Projects/C++/SteelEngine/bin/Resources/Textures/bricks2.jpg", &width, &height, &channels, 4);

        texture = Graphics::Material::Create("D:/Projects/C++/SteelEngine/bin/Resources/Textures/bricks2.jpg")->Setup();

        texture->Setup();

        return SE_TRUE;
    }

    void ImGUI_Editor::Draw(Graphics::ITexture* finalTexture)
    {
        ImGui::NewFrame();

        ImGui::SetNextWindowBgAlpha(-1.f);

        ImVec2 vWindowSize = ImGui::GetMainViewport()->Size;
		ImVec2 vPos0 = ImGui::GetMainViewport()->Pos;

        ImGui::SetNextWindowPos(vPos0, ImGuiCond_Always);
		ImGui::SetNextWindowSize(vWindowSize, ImGuiCond_Always);

        if(ImGui::Begin("EditorMainMenuBar", 0,
            ImGuiWindowFlags_MenuBar |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoTitleBar))
        {
            static const ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode;
			ImGuiID dockSpace = ImGui::GetID("MainWindowDockspace");

			ImGui::DockSpace(dockSpace, ImVec2(0.0f, 0.0f), dockspaceFlags);
            auto central = ImGui::DockBuilderGetCentralNode(dockSpace);

            ImGui::SetCursorPos(central->Pos);
            ImGui::Image((void*)finalTexture->GetTextureID(), central->Size, { 0, 1 }, { 1, 0 });

            if(ImGui::BeginMenuBar())
            {
                if(ImGui::BeginMenu("Project"))
                {
                    if(ImGui::MenuItem("New"))
                    {

                    }

                    SE_EDITOR_NOT_IMPLEMENTED_ADDNOTATION

                    if(ImGui::MenuItem("Load"))
                    {

                    }

                    SE_EDITOR_NOT_IMPLEMENTED_ADDNOTATION

                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }

            ImGui::End();
        }

        if(!GImGui/* || !GImGui->FrameScopeActive*/)
        {
            printf("Skipping frame!\n");

            return;
        }

        Render(*m_VirtualProjectVisualizer);

        if(m_CurrentScene == SceneType::START_MENU_SCENE)
        {
            for(Type::uint32 i = 0; i < m_StartMenuWindows.size(); i++)
            {
                HotReloader::InheritanceTrackKeeper* swapper = m_StartMenuWindows[i];
                const IReflectionData* type = swapper->GetType();
                EditorComponents::ImGUI::UserInterface* ui = swapper->Get<EditorComponents::ImGUI::UserInterface>();

                if(type->GetMetaData(EditorComponents::ImGUI::UserInterface::SEPARATE_WINDOW)->Convert<bool>())
                {
                    Variant* flagsVariant = type->GetMetaData(EditorComponents::ImGUI::UserInterface::FLAGS);

                    if(flagsVariant && flagsVariant->IsValid())
                    {
                        ImGui::Begin(ui->m_Title, 0, flagsVariant->Convert<ImGuiWindowFlags>());
                        {
                            ui->Draw();
                        }
                        ImGui::End();
                    }
                    else
                    {
                        ImGui::Begin(ui->m_Title);
                        {
                            ui->Draw();
                        }
                        ImGui::End();
                    }
                }
                else
                {
                    ui->Draw();
                }
            }
        }
        else if(m_CurrentScene == SceneType::EDITOR_SCENE)
        {
            for(Type::uint32 i = 0; i < m_UIs.size(); i++)
            {
                HotReloader::InheritanceTrackKeeper* swapper = m_UIs[i];
                const IReflectionData* type = swapper->GetType();
                EditorComponents::ImGUI::UserInterface* ui = swapper->Get<EditorComponents::ImGUI::UserInterface>();

                if(type->GetMetaData(EditorComponents::ImGUI::UserInterface::SEPARATE_WINDOW)->Convert<bool>())
                {
                    Variant* flagsVariant = type->GetMetaData(EditorComponents::ImGUI::UserInterface::FLAGS);

                    if(flagsVariant && flagsVariant->IsValid())
                    {
                        ImGui::Begin(ui->m_Title, 0, flagsVariant->Convert<ImGuiWindowFlags>());
                        {
                            ui->Draw();
                        }
                        ImGui::End();
                    }
                    else
                    {
                        ImGui::Begin(ui->m_Title);
                        {
                            ui->Draw();
                        }
                        ImGui::End();
                    }
                }
                else
                {
                    ui->Draw();
                }
            }

            for(Type::uint32 i = 0; i < m_MainEditorWindows.size(); i++)
            {
                HotReloader::InheritanceTrackKeeper* swapper = m_MainEditorWindows[i];
                const IReflectionData* type = swapper->GetType();
                EditorComponents::ImGUI::UserInterface* ui = swapper->Get<EditorComponents::ImGUI::UserInterface>();
                HotReloader::IRuntimeObject* runtime = swapper->Get<HotReloader::IRuntimeObject>();

                if(type->GetMetaData(EditorComponents::ImGUI::UserInterface::SEPARATE_WINDOW)->Convert<bool>())
                {
                    Variant* flagsVariant = type->GetMetaData(EditorComponents::ImGUI::UserInterface::FLAGS);

                    if(flagsVariant && flagsVariant->IsValid())
                    {
                        ImGui::Begin(ui->m_Title, 0, flagsVariant->Convert<ImGuiWindowFlags>());
                    }
                    else
                    {
                        ImGui::Begin(ui->m_Title);
                    }

                    {
                        ui->Draw();

                        const std::vector<IReflectionProperty*> a = type->GetProperties();

                        for(Type::uint32 i = 0; i < a.size(); i++)
                        {
                            Variant prop = type->GetProperty(a[i]->GetName().c_str(), runtime->m_Object);

                            if(prop.GetType() == m_FloatTypeID)
                            {
                                Variant* meta = a[i]->GetMetaData(ReflectionAttributes::RANGE);

                                if(meta->IsValid())
                                {
                                    Range range = meta->Convert<Range>();

                                    ImGui::SliderFloat(a[i]->GetName().c_str(), (float*)prop.GetAddress(), range.m_Min, range.m_Max);
                                }
                            }
                            else if(prop.GetType() == m_IntTypeID)
                            {
                                Variant* meta = a[i]->GetMetaData(ReflectionAttributes::RANGE);

                                if(meta->IsValid())
                                {
                                    Range range = meta->Convert<Range>();

                                    ImGui::SliderInt(a[i]->GetName().c_str(), (int*)prop.GetAddress(), range.m_Min, range.m_Max);
                                }
                            }
                        }
                    }
                    ImGui::End();
                }
                else
                {
                    ui->Draw();
                }
            }
        }

        static bool isAnyItemActive = false;

        if(ImGui::IsAnyItemActive() != isAnyItemActive)
        {
            isAnyItemActive = ImGui::IsAnyItemActive();

            m_NaiveManager->DispatchEvent(new AnyItemActiveChangedEvent(isAnyItemActive));
        }

        ImGui::Render();
    }

    void ImGUI_Editor::ProcessEvents(void* event)
    {
        m_API_Context->ProcessEvent(event);
    }

    void ImGUI_Editor::OnRecompile(HotReloader::IRuntimeObject* oldObject)
    {
        ImGui::SetCurrentContext(m_Context);
    }

    void ImGUI_Editor::OnEvent(Event::Naive* event)
    {

    }

    void ImGUI_Editor::operator()(const ChangeSceneEvent& event)
    {
        m_CurrentScene = event.m_SceneType;

        for(Type::uint32 i = 0; i < Reflection::GetTypesSize(); i++)
        {
            const IReflectionData* type = m_Types[i];

            bool found = false;

            for(HotReloader::InheritanceTrackKeeper* swapper : m_MainEditorWindows)
            {
                if(swapper->Get<HotReloader::IRuntimeObject>()->m_TypeID == type->GetTypeID())
                {
                    found = true;
                }
            }

            for(HotReloader::InheritanceTrackKeeper* swapper : m_StartMenuWindows)
            {
                if(swapper->Get<HotReloader::IRuntimeObject>()->m_TypeID == type->GetTypeID())
                {
                    found = true;
                }
            }

            if(!found)
            {
                std::vector<IReflectionInheritance*> inhs = type->GetInheritances();

                for(Type::uint32 i = 0; i < inhs.size(); i++)
                {
                    if(inhs[i]->GetTypeID() == Reflection::GetType<SteelEngine::EditorComponents::ImGUI::UserInterface>()->GetTypeID())
                    {
                        if(type->GetMetaData(Reflection::ReflectionAttribute::NETWORK_COMMAND)->Convert<bool>())
                        {
                            Network::INetworkManager* manager =
                                Reflection::GetType("SteelEngine::Core")->GetMetaData(Core::GlobalSystems::NETWORK_MANAGER)->Convert<Network::INetworkManager*>();
                            std::vector<HotReloader::InheritanceTrackKeeper*> commands = manager->GetCommands();

                            for(HotReloader::InheritanceTrackKeeper* cmd : commands)
                            {
                                HotReloader::IRuntimeObject* runtime = cmd->Get<HotReloader::IRuntimeObject>();

                                if(type->GetTypeID() == runtime->m_TypeID)
                                {
                                    EditorComponents::ImGUI::UserInterface* ui = cmd->Get<EditorComponents::ImGUI::UserInterface>();

                                    strcpy(ui->m_Title, type->GetTypeName());

                                    ui->m_Context = m_Context;

                                    ui->Init();

                                    m_UIs.push_back(cmd);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

}}}