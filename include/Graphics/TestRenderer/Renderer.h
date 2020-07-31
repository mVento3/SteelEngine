#pragma once

#include "Graphics/IRenderer.h"

#include "Event/EventObserver.h"

#include "Platform/Graphics/RenderDevice.h"

#include "Graphics/TestRenderer/Renderer.Generated.h"

namespace SteelEngine { namespace Graphics { namespace TestRenderer {

    SE_CLASS(
        Reflection::ReflectionAttribute::RUNTIME_SERIALIZE,
        Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS,
        Reflection::ReflectionAttribute::HOT_RELOAD
    )
    class Renderer : public IRenderer, public EventObserver
    {
        GENERATED_BODY
    public:
        enum Uniform
        {
            SAMPLER_POSITION,
            SAMPLER_ALBEDO,

            SIZE
        };

    private:
        SteelEngine::Utils::RenderContext* m_RenderContext;
        IWindow* m_Window;

        ITexture* m_AlbedoTexture;
        IFramebuffer* m_GBuffer;
        IMesh* m_QuadMesh;
        IShader* m_QuadShader;

    public:
        Renderer();
        ~Renderer();

        Result Init() override;
        void Update() override;
        void Cleanup() override;

        void PreRender() override;
        void Render(entt::registry& scene) override;
        void PostRender() override;

        void OnEvent(Event::Naive* event) override;

        void* GetFinalTexture() override;

        void SetRenderTarget(IWindow* window) override;
        void SetSceneManager(ISceneManager* scene) override;
        void SetRenderContext(SteelEngine::Utils::RenderContext* context) override;
    };

}}}