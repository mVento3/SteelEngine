#include "Utils/Graphics/Renderer3D.h"

namespace SteelEngine { namespace Graphics { namespace Utils {

    RuntimeDatabase* Renderer3D::sm_RuntimeDatabase;
    SteelEngine::Utils::RenderContext* Renderer3D::sm_Context;

    Renderer3D::Renderer3D()
    {

    }

    Renderer3D::~Renderer3D()
    {

    }

    void Renderer3D::AddModel(IModel* model, const Transform& transform, bool castShadow)
    {
        sm_Context->GetCurrentRenderer()->AddModel(model, sm_Context->GetSceneManager(), transform, castShadow);
    }

    void Renderer3D::InitializeInCurrentModule()
    {
    // TODO: Maybe this should doing automatically

        if(sm_RuntimeDatabase == NULL)
        {
            sm_RuntimeDatabase = (RuntimeDatabase*)ModuleManager::GetModule("RuntimeDatabase");
        }

        if(sm_Context == NULL)
        {
            sm_Context = sm_RuntimeDatabase->m_RenderContextDatabase->m_Context;
        }
    }

// API
    void Renderer3D::Draw(float* vertices, size_t vSize, uint32_t* indices, size_t iSize)
    {
        
    }

    void Renderer3D::Draw(SteelEngine::Graphics::IMesh* mesh)
    {
        
    }

}}}