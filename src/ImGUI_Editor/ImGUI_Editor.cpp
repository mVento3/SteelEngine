#include "ImGUI_Editor/ImGUI_Editor.h"

#include "imgui/imgui_internal.h"

#include "algorithm"

#include "RuntimeReflection/Reflection.h"

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
        m_Context = (ImGuiContext*)Reflection::GetType("SteelEngine::VulkanContext")->Invoke("GetContext", context).Convert<void*>();

        ImGui::SetCurrentContext(m_Context);

        std::vector<IReflectionData*> types = Reflection::GetTypes();

        for(Type::uint32 i = 0; i < types.size(); i++)
        {
            IReflectionData* type = types[i];
            std::vector<IReflectionInheritance*> inhs = type->GetInheritances();

            for(Type::uint32 i = 0; i < inhs.size(); i++)
            {
                if(inhs[i]->GetTypeID() == typeid(Window).hash_code())
                {
                    Window** window = (Window**)&type->Create()->m_Object;

                    (*window)->m_Title = type->GetTypeName();
                    (*window)->m_Context = m_Context;

                    if(type->GetMetaData(ReflectionAttributes::EDITOR_WINDOW)->Convert<bool>())
                    {
                        if(type->GetMetaData(ReflectionAttributes::SCENE_TYPE)->Convert<SceneType>() & SceneType::EDITOR_SCENE)
                        {
                            m_Windows.push_back(window);
                        }

                        if(type->GetMetaData(ReflectionAttributes::SCENE_TYPE)->Convert<SceneType>() & SceneType::START_MENU_SCENE)
                        {
                            m_StartMenuWindows.push_back(window);
                        }
                    }
                    else
                    {
                        m_NonWindows.push_back(window);
                    }

                    (*window)->Init();
                }
            }
        }

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

        if(!GImGui || !GImGui->FrameScopeActive)
        {
            printf("Skipping frame!\n");

            return;
        }

        if(m_CurrentScene == SceneType::START_MENU_SCENE)
        {
            for(Type::uint32 i = 0; i < m_StartMenuWindows.size(); i++)
            {
                Window* window = *m_StartMenuWindows[i];

                ImGui::Begin(window->m_Title.c_str());
                {
                    window->Draw();
                }
                ImGui::End();
            }
        }
        else if(m_CurrentScene == SceneType::EDITOR_SCENE)
        {
            for(Type::uint32 i = 0; i < m_Windows.size(); i++)
            {
                Window* window = *m_Windows[i];

                ImGui::Begin(window->m_Title.c_str());
                {
                    window->Draw();

                    const std::vector<IReflectionData::PropertyInfo> a = Reflection::GetType(window)->GetProperties();

                    for(Type::uint32 i = 0; i < a.size(); i++)
                    {
                        IReflectionData::PropertyInfo info = a[i];
                        Variant prop = Reflection::GetType(window)->GetProperty(info.m_Name, window);

                        if(prop.GetType() == m_FloatTypeID)
                        {
                            Variant* meta = info.m_Property->GetMetaData(ReflectionAttributes::RANGE);

                            if(meta->IsValid())
                            {
                                Range range = meta->Convert<Range>();

                                ImGui::SliderFloat(info.m_Name.c_str(), (float*)prop.GetAddress(), range.m_Min, range.m_Max);
                            }
                        }
                        else if(prop.GetType() == m_IntTypeID)
                        {
                            Variant* meta = info.m_Property->GetMetaData(ReflectionAttributes::RANGE);

                            if(meta->IsValid())
                            {
                                Range range = meta->Convert<Range>();

                                ImGui::SliderInt(info.m_Name.c_str(), (int*)prop.GetAddress(), range.m_Min, range.m_Max);
                            }
                        }
                    }
                }
                ImGui::End();
            }
        }

        for(Window** wnd : m_NonWindows)
        {
            (*wnd)->Draw();
        }

        ImGui::Render();
    }

    void ImGUI_Editor::ProcessEvents(void* event)
    {
        m_API_Context->ProcessEvent(event);
    }

    void ImGUI_Editor::operator()(const RecompiledEvent& event)
    {
        ImGui::SetCurrentContext(m_Context);

        if(Reflection::GetType(event.m_NewObject)->GetMetaData(Editor::ReflectionAttributes::EDITOR_WINDOW)->Convert<bool>())
        {
            bool found = false;

            for(Window** wnd : m_Windows)
            {
                if((*wnd)->m_TypeID == event.m_NewObject->m_TypeID)
                {
                    found = true;
                }
            }

            for(Window** wnd : m_StartMenuWindows)
            {
                if((*wnd)->m_TypeID == event.m_NewObject->m_TypeID)
                {
                    found = true;
                }
            }

            if(!found)
            {
                Window** wnd = (Window**)&event.m_NewObject->m_Object;

                m_Windows.push_back(wnd);

                (*wnd)->m_Title = Reflection::GetType(event.m_NewObject)->GetTypeName();
                (*wnd)->m_Context = m_Context;
            }
        }
    }

    void ImGUI_Editor::operator()(const ChangeSceneEvent& event)
    {
        m_CurrentScene = event.m_SceneType;

        std::vector<IReflectionData*> types = Reflection::GetTypes();

        for(Type::uint32 i = 0; i < types.size(); i++)
        {
            IReflectionData* type = types[i];

            bool found = false;

            for(Window** wnd : m_Windows)
            {
                if((*wnd)->m_TypeID == type->GetTypeID())
                {
                    found = true;
                }
            }

            for(Window** wnd : m_StartMenuWindows)
            {
                if((*wnd)->m_TypeID == type->GetTypeID())
                {
                    found = true;
                }
            }

            if(!found && type->GetMetaData(ReflectionAttributes::EDITOR_WINDOW)->Convert<bool>())
            {
                Window** wnd = (Window**)&type->Create()->m_Object;

                m_Windows.push_back(wnd);

                (*wnd)->m_Title = type->GetTypeName();
                (*wnd)->m_Context = m_Context;

                (*wnd)->Init();

                break;
            }
        }
    }

}}}