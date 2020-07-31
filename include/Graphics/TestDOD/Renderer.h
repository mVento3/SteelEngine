#pragma once

#include "Graphics/IRenderer.h"

#include "Event/EventObserver.h"

#include "PlatformDll/Graphics/RenderDeviceDOD.h"

#include "Graphics/TestDOD/Renderer.Generated.h"

namespace SteelEngine { namespace Graphics { namespace TestDOD {

    SE_CLASS(
        Reflection::ReflectionAttribute::RUNTIME_SERIALIZE,
        Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS,
        Reflection::ReflectionAttribute::HOT_RELOAD
    )
    class Renderer : public IRenderer, public EventObserver
    {
        GENERATED_BODY
    private:
        IWindow* m_Window;
        ISceneManager* m_Scene;
        SteelEngine::Utils::RenderContext* m_RenderContext;
        Platform::Graphics::RenderDeviceDOD* m_GFX;

        SteelEngine::Platform::Graphics::se_resource_command_buffer m_CommandBuffer;
        Platform::Graphics::se_resource_texture texture;
        Platform::Graphics::se_resource_shader shader;
        Platform::Graphics::se_resource_texture resultTexture;
        Platform::Graphics::se_resource_frame_buffer result;
        Platform::Graphics::se_resource_vertex_buffer vb;
        Platform::Graphics::se_resource_uniform color;
        Platform::Graphics::se_resource_uniform brick;

    public:
        Renderer();
        ~Renderer();

        Result Init() override;
        void Update() override;
        void Cleanup() override;

        void PreRender() override;
        void Render(entt::registry& scene) override;
        void PostRender() override;

        void* GetFinalTexture() override;

        void SetRenderTarget(IWindow* window) override;
        void SetSceneManager(ISceneManager* scene) override;
        void SetRenderContext(SteelEngine::Utils::RenderContext* context) override;

        void OnEvent(Event::Naive* event) override;
    };

}}}