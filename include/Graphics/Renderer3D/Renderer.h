#pragma once

#include "Graphics/IRenderer.h"

#include "GL/glew.h"

#include "Event/EventObserver.h"

#include "RuntimeReflection/Macro.h"

#include "Graphics/Renderer3D/Texture.h"
#include "Graphics/Renderer3D/Framebuffer.h"
#include "Graphics/Renderer3D/Shader.h"
#include "Graphics/Renderer3D/MeshData.h"

#include "Editor/ImGUI/Events/AnyItemActiveChangedEvent.h"

#include "Math/Camera.h"

#include "Graphics/Model.h"

#include "Input/Events/KeyDownEvent.h"
#include "Input/Events/KeyUpEvent.h"
#include "Input/Events/MouseMotionEvent.h"
#include "Input/Events/ChangeMousePositionEvent.h"

#include "Graphics/Renderer3D/ECS_Components/VAO_Component.h"
#include "Graphics/Renderer3D/ECS_Components/TextureComponent.h"

#include "Graphics/Renderer3D/Renderer.Generated.h"

namespace SteelEngine { namespace Graphics { namespace Renderer3D {

    SE_CLASS(
        Reflection::ReflectionAttribute::RUNTIME_SERIALIZE,
        Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS,
        Reflection::ReflectionAttribute::HOT_RELOAD
    )
    class Renderer : public IRenderer, public EventObserver
    {
        GENERATED_BODY
    public:
        enum MeshVBO
        {
            POSITION_VB,
            TEXCOORD_VB,
            NORMAL_VB,
            INDEX_VB,

            SIZE_VB
        };

        enum Uniform
        {
        // General
            MODEL_U,
            VIEW_U,
            PROJECTION_U,

        // For light
            EYE_POSITION,

        // Material
            MATERIAL_SHININESS,

        // Directional light
            DIRECTIONAL_LIGHT_DIRECTION,
            DIRECTIONAL_LIGHT_AMBIENT,
            DIRECTIONAL_LIGHT_DIFFUSE,
            DIRECTIONAL_LIGHT_SPECULAR,

            SIZE_U
        };

    private:
        Texture* m_FinalTexture;
        Framebuffer* m_GBuffer;

        IWindow* m_Window;
        Shader* m_GShader;
        Camera* m_Camera;

        GLuint m_Uniforms[Uniform::SIZE_U];
        Texture* m_SomeTexture;

        bool m_Keys[256] = { false };
        bool m_Controlls;
        bool m_RotateCamera;

        void InitOpenGL();

        void Create(
            const std::vector<glm::vec3>& vertices,
            const std::vector<glm::vec2>& texCoords,
            const std::vector<glm::vec3>& normals,
            const std::vector<uint32_t>& indices,
            const Transform& trans
        );

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

        void* GetFinalTexture() override;

        void SetRenderTarget(IWindow* window) override;
        void SetSceneManager(ISceneManager* scene) override { m_Scene = scene; }

        void OnEvent(Event::Naive* event) override;
    };

}}}