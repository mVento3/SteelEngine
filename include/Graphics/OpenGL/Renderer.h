#pragma once

#include "Graphics/IRenderer.h"

#include "GL/glew.h"

#include "Window/IWindow.h"

#include "Graphics/OpenGL/Shader.h"
#include "Graphics/OpenGL/Mesh.h"
#include "Graphics/OpenGL/Texture.h"

#include "Graphics/OpenGL/Renderer.Generated.h"

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

    public:
        Renderer(IWindow* window);
        ~Renderer();

        Result Init() override;
        void Update() override;
        void Cleanup() override;

        void PreRender() override;
        void Render() override;
        void PostRender() override;
    };

}}}