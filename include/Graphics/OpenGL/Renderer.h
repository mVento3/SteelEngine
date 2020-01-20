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

#include "Graphics/Math/Transform.h"
#include "Graphics/Math/Camera.h"

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

#include "Graphics/OpenGL/Renderer.Generated.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    SE_CLASS(
        SteelEngine::ReflectionAttribute::RUNTIME_SERIALIZE,
        SteelEngine::ReflectionAttribute::GENERATE_CAST_FUNCTIONS
    )
    class Renderer : public IRenderer, public EventObserver
    {
        GENERATED_BODY
    private:
        IWindow* m_Window;

        Camera* m_Camera;

        Shader* m_QuadShader;
        QuadMesh* m_QuadMesh;

        std::vector<Model> m_Models;
        std::vector<Light*> m_Lights;

        Mesh* m_Mesh;
        Mesh* m_Mesh2;
        Mesh* m_Mesh3;
        Texture* m_Texture;
        Texture* m_NormalMapTexture;
        Texture* m_DispMapTexture;
        Transform m_Trans;
        Transform m_Trans2;
        Transform m_Trans3;

        Shader* m_G_Shader;
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

        float m_Counter;

        bool m_Keys[256] = { false };
        bool m_RotateCamera;

        bool m_Controlls;

    public:
        Renderer(IWindow* window);
        ~Renderer();

        Result Init() override;
        void Update() override;
        void Cleanup() override;

        void PreRender() override;
        void Render() override;
        void PostRender() override;

        void OnEvent(Event::Naive* event) override;
    };

}}}