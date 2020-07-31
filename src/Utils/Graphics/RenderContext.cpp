#include "Utils/Graphics/RenderContext.h"

#include "Graphics/OpenGL/Renderer.h"

// #include "Graphics/Vulkan/Renderer.h"

#include "Graphics/Renderer2D/Renderer.h"

#include "Graphics/Renderer3D/Renderer.h"

#include "Logger/Logger.h"

#include "RuntimeReflection/Reflection.h"

#include "Core/Core.h"

#include "Utils/Reflection.h"

#include "PlatformDll/Graphics/RenderDeviceDOD.h"

namespace SteelEngine { namespace Utils {

    RuntimeDatabase* RenderContext::sm_RuntimeDB;

    RenderContext::RenderContext()
    {
        m_CurrentRenderer = 0;
        m_CurrentAPI = Graphics::IRenderer::API::NONE;
    }

    RenderContext::~RenderContext()
    {

    }

    bool RenderContext::Initialize()
    {
    // We should search through some meta for APIs

        if(sm_RuntimeDB == NULL)
        {
            sm_RuntimeDB = (RuntimeDatabase*)ModuleManager::GetModule("RuntimeDatabase");
        }

        if(GetRenderContext(sm_RuntimeDB) == NULL)
        {
            SetRenderContext(sm_RuntimeDB, this);
        }

        // IReflectionData const* const* types = Reflection::GetTypes();
        // Type::uint32 size = Reflection::GetTypesSize();

        // for(Type::uint32 i = 0; i < size; i++)
        // {
        //     const IReflectionData* type = types[i];
        //     Variant* renderAPI_Variant = type->GetMetaData(Reflection::ReflectionAttribute::RENDER_API);

        //     if(renderAPI_Variant && renderAPI_Variant->IsValid())
        //     {
        //         IRenderer::API api = renderAPI_Variant->Convert<IRenderer::API>();

        //         if(api == m_CurrentAPI)
        //         {
        //             m_CurrentRenderer = (IRenderer*)type->Create();

        //             ((OpenGL::Renderer*)m_CurrentRenderer)->m_Scene = m_Scene;
        //         }
        //     }
        // }

        // if(sm_RuntimeDB == NULL)
        // {
        //     sm_RuntimeDB = (RuntimeDatabase*)ModuleManager::GetModule("RuntimeDatabase");
        // }

        // if(GetRenderContext(sm_RuntimeDB) == NULL)
        // {
        //     SetRenderContext(sm_RuntimeDB, this);

        //     if(m_CurrentRenderer == NULL)
        //     {
        //         switch(sm_CurrentAPI)
        //         {
        //         case API::OPENGL:
        //             m_CurrentRenderer = new OpenGL::Renderer();

        //             ((OpenGL::Renderer*)m_CurrentRenderer)->m_Scene = m_Scene;
        //             break;
        //         case API::RENDERER_2D:
        //             m_CurrentRenderer = new Renderer2D::Renderer();

        //             ((Renderer2D::Renderer*)m_CurrentRenderer)->m_Scene = m_Scene;
        //             break;
        //         case API::RENDERER_3D:
        //             m_CurrentRenderer = new Renderer3D::Renderer();

        //             ((Renderer3D::Renderer*)m_CurrentRenderer)->m_Scene = m_Scene;
        //             break;
        //         // case API::VULKAN:
        //         //     m_CurrentRenderer = new Vulkan::Renderer();
        //         //     break;
        //         case API::NONE:
        //             break;
        //         default:
        //             SE_ERROR("Specified render API does not exists!");
        //             break;
        //         }
        //     }
        // }
        // else
        // {
        //     m_CurrentRenderer = GetRenderContext(sm_RuntimeDB)->GetCurrentRenderer();
        // }

        return true;
    }

    void RenderContext::SelectRenderer(const char* name)
    {
        IReflectionData const* const* types = Reflection::GetTypes();
        Type::uint32 size = Reflection::GetTypesSize();

        for(Type::uint32 i = 0; i < size; i++)
        {
            const IReflectionData* type = types[i];

            if(getFullTypename(type) == name)
            {
                m_CurrentRenderer = (Graphics::IRenderer*)type->Create();
                m_SelectedRendererName = name;

                m_CurrentRenderer->SetSceneManager(m_Scene);

                EventObserver* eve = type->Invoke("Cast_EventObserver", GetCurrentRenderer()).Convert<EventObserver*>();

                Reflection::GetType("SteelEngine::Core")->GetMetaData(Core::GlobalSystems::EVENT_MANAGER)->Convert<IEventManager*>()->AddEventObserver(eve);
            }
        }
    }

    void RenderContext::SelectRenderer(size_t typeID)
    {
        IReflectionData const* const* types = Reflection::GetTypes();
        Type::uint32 size = Reflection::GetTypesSize();

        for(Type::uint32 i = 0; i < size; i++)
        {
            const IReflectionData* type = types[i];

            if(type->GetTypeID() == typeID)
            {
                m_CurrentRenderer = (Graphics::IRenderer*)type->Create();
                m_SelectedRendererName = getFullTypename(type).c_str();

                m_CurrentRenderer->SetSceneManager(m_Scene);
            }
        }
    }

    void RenderContext::SelectPlatform(SteelEngine::Graphics::IRenderer::API api)
    {
        if(m_CurrentAPI != api)
        {
            m_CurrentAPI = api;
            m_RenderDevice = Graphics::RenderDevice::Create();

            void* dll;
            SteelEngine::Platform::Graphics::render_device_construct_callback cons;
            SteelEngine::Module::load("D:/Projects/C++/SteelEngine/bin/PlatformDll.dll", &dll);

            SteelEngine::Module::get("render_device_construct", dll, (void**)&cons);

            m_RenderDeviceDOD = cons(m_CurrentAPI);

        // Here we should reinitialize renderer or smth...
        }
    }

}}