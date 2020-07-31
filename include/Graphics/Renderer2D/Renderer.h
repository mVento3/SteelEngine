#pragma once

#include "Graphics/IRenderer.h"

#include "Graphics/Renderer2D/Framebuffer.h"
#include "Graphics/Renderer2D/Texture.h"
#include "Graphics/Renderer2D/Shader.h"
#include "Graphics/Renderer2D/Vertex.h"

#include "RuntimeReflection/Macro.h"

#include "Event/EventObserver.h"

#include "GL/glew.h"

#include "RuntimeReflection/Reflection.h"

#include "Input/Events/KeyDownEvent.h"
#include "Input/Events/KeyUpEvent.h"
#include "Input/Events/MouseWheelEvent.h"

#include "Graphics/Events/ViewportSizeChangedEvent.h"

#include "Graphics/Renderer2D/ECS_Components/QuadComponent.h"

#include "Graphics/Renderer2D/Renderer.Generated.h"

namespace SteelEngine { namespace Graphics { namespace Renderer2D {

    SE_CLASS(
        Reflection::ReflectionAttribute::RUNTIME_SERIALIZE,
        Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS,
        Reflection::ReflectionAttribute::HOT_RELOAD
    )
    class Renderer : public IRenderer, public EventObserver
    {
        GENERATED_BODY
    public:
        const static Type::uint32 MAX_QUAD_COUNT;
        const static Type::uint32 MAX_VERTEX_COUNT;
        const static Type::uint32 MAX_INDEX_COUNT;

        enum ShaderUniformLocation
        {
            VIEW,
            PROJECTION,

            SIZE
        };

    private:
        IWindow* m_Window;

        Texture* m_FinalTexture;
        Framebuffer* m_G_Framebuffer;

        GLuint m_VAO;
        GLuint m_VBO;
        GLuint m_IBO;
        GLuint m_Uniforms[ShaderUniformLocation::SIZE];

        Vertex* m_Vertices;
        Vertex* m_Buffer;
        Type::uint32 m_DrawIndexCount;
        glm::vec4 m_QuadVertexPositions[4];

        Shader* m_Shader;

        glm::mat4 m_Projection;
        glm::mat4 m_Model;
        glm::mat4 m_View;

        glm::vec2 m_CameraPosition;
        bool m_Keys[256] = { false };
        float m_Scale;
        int m_Width;
        int m_Height;

        Texture* m_TestTexture1;
        Texture* m_TestTexture2;

        Variant* m_DeltaTimeVariant;

        entt::entity test;

    // Statistics
        Type::uint32 m_DrawCalls;

        entt::entity AddQuad(float x, float y, int texID);

        void Flush(Type::uint32 indexCount, Type::uint32 verticesSize);

    public:
        Renderer();
        ~Renderer();

        ISceneManager* m_Scene;

        Result Init() override;
        void Update() override;
        void Cleanup() override;

        void PreRender() override;
        void Render(entt::registry& scene) override;
        void PostRender() override;

        void SetRenderTarget(IWindow* window) override { m_Window = window; }
        void SetSceneManager(ISceneManager* scene) override { m_Scene = scene; }

        void* GetFinalTexture() override;

        void OnEvent(Event::Naive* event) override;

        void operator()(const ViewportSizeChangedEvent& event);
    };

}}}