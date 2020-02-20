#pragma once

#include "Graphics/IRenderer.h"

#include "GL/glew.h"

#include "Window/IWindow.h"

#include "Graphics/OpenGL/Shader.h"
#include "Graphics/OpenGL/Mesh.h"
#include "Graphics/OpenGL/Texture.h"
#include "Graphics/OpenGL/Framebuffer.h"
#include "Graphics/OpenGL/QuadMesh.h"
#include "Graphics/OpenGL/ShadowInfo.h"
#include "Graphics/OpenGL/Model.h"
#include "Graphics/OpenGL/TestShader.h"

#include "Math/Transform.h"
#include "Math/Camera.h"

#include "Graphics/OpenGL/Lights/SpotLight.h"
#include "Graphics/OpenGL/Lights/DirectionalLight.h"

#include "Input/Events/ChangeMousePositionEvent.h"
#include "Input/Events/KeyDownEvent.h"
#include "Input/Events/KeyUpEvent.h"
#include "Input/Events/MouseMotionEvent.h"

#include "Window/Events/WindowResizedEvent.h"

#include "Event/GlobalEvent.h"
#include "Event/EventObserver.h"

#include "ImGUI_Editor/Events/AnyItemActiveChangedEvent.h"

#include "vector"

#include "Graphics/OpenGL/ECS_Components/RenderableComponent.h"

#include "Graphics/ECS_Components/TransformComponent.h"

#include "Graphics/OpenGL/Renderer.Generated.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    SE_CLASS(
        SteelEngine::Reflection::ReflectionAttribute::RUNTIME_SERIALIZE,
        SteelEngine::Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS,
        Reflection::ReflectionAttribute::HOT_RELOAD
    )
    class Renderer : public IRenderer, public EventObserver
    {
        GENERATED_BODY
    private:
        IWindow* m_Window;

        Camera* m_Camera;

        Shader* m_QuadShader;
        QuadMesh* m_QuadMesh;

        std::vector<Light*> m_Lights;

        Texture* m_Texture;
        Texture* m_NormalMapTexture;
        Texture* m_DispMapTexture;

        TestShader* m_G_Shader;
        Texture* m_PositionTexture;
        Texture* m_NormalTexture;
        Texture* m_AlbedoTexture;
        Texture* m_ShadowTexture;
        Framebuffer* m_G_Buffer;

        Shader* m_ShadowShader;
        Camera* m_ShadowCamera;

        SpotLight* m_SpotLight;
        Quaternion m_SpotRotation;
        DirectionalLight* m_DirectionalLight;

        Transform m_DirectionalLightTransform;

        bool m_Keys[256] = { false };
        bool m_RotateCamera;

        bool m_Controlls;

        entt::entity AddModel(IMesh* mesh, entt::registry* scene, const Transform& transform, bool castShadow = true) override;

    public:
        Renderer(IWindow* window);
        ~Renderer();

        Result Init() override;
        void Update() override;
        void Cleanup() override;

        void PreRender() override;
        void Render(entt::registry* scene) override;
        void PostRender() override;

        void OnEvent(Event::Naive* event) override;
    };

}}}