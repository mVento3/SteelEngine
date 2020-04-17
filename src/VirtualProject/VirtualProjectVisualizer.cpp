#include "VirtualProject/VirtualProjectVisualizer.h"

#include "imgui/imgui.h"

#include "RuntimeReflection/Reflection.h"

#include "VirtualProject/VirtualProject.h"

#include "EditorComponents/ImGUI/UserInterface.h"

#include "HotReloader/InheritanceTrackKeeper.h"

namespace SteelEngine {

    VirtualProjectVisualizer::VirtualProjectVisualizer(VirtualProject** virtualProject) :
        m_VirtualProject(virtualProject),
        m_UserInterfaceTypeID(typeid(EditorComponents::ImGUI::UserInterface).hash_code())
    {
        m_Loaded = false;
    }

    VirtualProjectVisualizer::~VirtualProjectVisualizer()
    {

    }

    void VirtualProjectVisualizer::Render()
    {
        if(!m_Loaded)
        {
            return;
        }

        VirtualProject* vp = (*m_VirtualProject);

        for(Type::uint32 i = 0; i < vp->m_ProjectScripts.size(); i++)
        {
            HotReloader::InheritanceTrackKeeper* swapper = vp->m_ProjectScripts[i];
            HotReloader::IRuntimeObject* obj = swapper->Get<HotReloader::IRuntimeObject>();
            IReflectionData* type = Reflection::GetType(obj);
            const Vector<IReflectionInheritance>& inhs = type->GetInheritances();

            for(Type::uint32 j = 0; j < inhs.Size(); j++)
            {
                if(inhs[j]->GetTypeID() == m_UserInterfaceTypeID)
                {
                    EditorComponents::ImGUI::UserInterface* ui = swapper->Get<EditorComponents::ImGUI::UserInterface>();

                    if(Reflection::GetType(obj)->GetMetaData(EditorComponents::ImGUI::UserInterface::SEPARATE_WINDOW)->Convert<bool>())
                    {
                        ImGui::Begin(ui->m_Title);
                        {
                            ui->DrawUser();
                        }
                        ImGui::End();
                    }
                    else
                    {
                        ui->DrawUser();
                    }

                    break;
                }
            }
        }
    }

    void VirtualProjectVisualizer::Init(IContext* context)
    {
        m_Context = context;

        ImGui::SetCurrentContext(Reflection::GetType("SteelEngine::OpenGL_Context")->Invoke("GetContext", context).Convert<ImGuiContext*>());
    }

    void VirtualProjectVisualizer::OnProjectLoad()
    {
        VirtualProject* vp = (*m_VirtualProject);
        const size_t typeID = typeid(EditorComponents::ImGUI::UserInterface).hash_code();

        for(Type::uint32 i = 0; i < vp->m_ProjectScripts.size(); i++)
        {
            HotReloader::InheritanceTrackKeeper* swapper = vp->m_ProjectScripts[i];
            HotReloader::IRuntimeObject* obj = swapper->Get<HotReloader::IRuntimeObject>();
            IReflectionData* type = Reflection::GetType(obj);
            const Vector<IReflectionInheritance>& inhs = type->GetInheritances();

            for(Type::uint32 j = 0; j < inhs.Size(); j++)
            {
                if(inhs[j]->GetTypeID() == typeID)
                {
                    EditorComponents::ImGUI::UserInterface* ui = swapper->Get<EditorComponents::ImGUI::UserInterface>();

                    strcpy(ui->m_Title, type->GetTypeName());
                    ui->m_Context = Reflection::GetType("SteelEngine::OpenGL_Context")->Invoke("GetContext", m_Context).Convert<ImGuiContext*>();

                    ui->Init();

                    break;
                }
            }
        }

        m_Loaded = true;
    }

}