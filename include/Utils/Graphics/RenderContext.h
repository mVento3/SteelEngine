#pragma once

#include "Graphics/IRenderer.h"

#include "RuntimeReflection/Macro.h"

namespace SteelEngine {

    struct ICore;

    namespace Graphics {

        class RenderDevice;

    }

    namespace Platform { namespace Graphics {

        struct RenderDeviceDOD;

    }}

    namespace Utils {

// In general render context should keep possibility to change between renderers and graphics api's

    SE_CLASS()
    class RenderContext
    {
        friend struct ICore;
    private:
    // Renderer is custom render pipeline defined by user
        SteelEngine::Graphics::IRenderer* m_CurrentRenderer;
        ISceneManager* m_Scene;
        static RuntimeDatabase* sm_RuntimeDB;
    // API will be set while seting or changing renderer
    // Update: IDK about that...
        SteelEngine::Graphics::IRenderer::API m_CurrentAPI;
        const char* m_SelectedRendererName;
    // Render device will keep all implementations of various graphics api's like OpenGL, Vulkan, DirectX etc.
        SteelEngine::Graphics::RenderDevice* m_RenderDevice;
        SteelEngine::Platform::Graphics::RenderDeviceDOD* m_RenderDeviceDOD;

        void SetSceneManager(ISceneManager* scene) { m_Scene = scene; }

        void SetRenderContext(RuntimeDatabase* db, RenderContext* context)
        {
            db->m_RenderContextDatabase->m_Context = context;
        }

        static RenderContext* GetRenderContext(RuntimeDatabase* db)
        {
            return db->m_RenderContextDatabase->m_Context;
        }

    public:
        RenderContext();
        ~RenderContext();

        bool Initialize();

        void SelectRenderer(const char* name);
        void SelectRenderer(size_t typeID);
        void SelectPlatform(SteelEngine::Graphics::IRenderer::API api);

        SteelEngine::Graphics::IRenderer* GetCurrentRenderer() const { return m_CurrentRenderer; }
        ISceneManager* GetSceneManager() const { return m_Scene; }
        SteelEngine::Graphics::RenderDevice* GetRenderDevice() const { return m_RenderDevice; }
        SteelEngine::Platform::Graphics::RenderDeviceDOD* GetRenderDeviceDOD() const { return m_RenderDeviceDOD; }

        static SteelEngine::Graphics::IRenderer::API GetCurrentAPI()
        {
            if(sm_RuntimeDB == NULL)
            {
                sm_RuntimeDB = (RuntimeDatabase*)ModuleManager::GetModule("RuntimeDatabase");
            }

            return ((RenderContext*)GetRenderContext(sm_RuntimeDB))->m_CurrentAPI;
        }

        static const char* GetRendererName()
        {
            if(sm_RuntimeDB == NULL)
            {
                sm_RuntimeDB = (RuntimeDatabase*)ModuleManager::GetModule("RuntimeDatabase");
            }

            return ((RenderContext*)GetRenderContext(sm_RuntimeDB))->m_SelectedRendererName;
        }
    };

}}