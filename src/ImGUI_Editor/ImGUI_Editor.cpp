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

        return SE_TRUE;
    }

    void ImGUI_Editor::Draw()
    {
        ImGui::NewFrame();

        if(ImGui::BeginMainMenuBar())
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

            ImGui::EndMainMenuBar();
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