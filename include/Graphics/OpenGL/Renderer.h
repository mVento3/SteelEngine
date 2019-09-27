#pragma once

#include "Graphics/IRenderer.h"

#include "GL/glew.h"

#include "Window/IWindow.h"

#include "Graphics/OpenGL/Shader.h"
#include "Graphics/OpenGL/Mesh.h"
#include "Graphics/OpenGL/Texture.h"
#include "Graphics/OpenGL/Transform.h"
#include "Graphics/OpenGL/Camera.h"

#include "Graphics/OpenGL/Renderer.Generated.h"

#include "Input/Events/KeyDownEvent.h"
#include "Input/Events/KeyUpEvent.h"
#include "Input/Events/MouseMotionEvent.h"
#include "Input/Events/ChangeMousePositionEvent.h"

#include "Event/GlobalEvent.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    SE_CLASS(
        SteelEngine::ReflectionAttribute::RUNTIME_SERIALIZE
    )
    class Renderer : public IRenderer
    {
        GENERATED_BODY
    private:
        IWindow* m_Window;

        Shader* m_Shader;
        Mesh* m_Mesh;
        Texture* m_Texture;
        Transform m_Trans;
        Camera* m_Camera;

        float m_Counter;

        bool m_Keys[256] = { false };
        bool m_RotateCamera;

    public:
        Renderer(IWindow* window);
        ~Renderer();

        Result Init() override;
        void Update() override;
        void Cleanup() override;

        void PreRender() override;
        void Render() override;
        void PostRender() override;

        void operator()(const KeyDownEvent& event);
        void operator()(const KeyUpEvent& event);
        void operator()(const MouseMotionEvent& event);
    };

}}}