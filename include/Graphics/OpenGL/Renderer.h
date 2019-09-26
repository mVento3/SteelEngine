#pragma once

#include "Graphics/IRenderer.h"

#include "GL/glew.h"

#include "Window/IWindow.h"

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

    public:
        Renderer(IWindow* window);
        ~Renderer();

        Result Init() override;
        void Update() override;
        void Cleanup() override;

        void PreRender() override;
        void Render() override;
        void PostRender() override;

        SE_METHOD()
        Result GlewInit();
    };

}}}