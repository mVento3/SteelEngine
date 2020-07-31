#pragma once

#include "Graphics/IShader.h"
#include "Graphics/IRenderer.h"

#include "Platform/Graphics/ShaderUniform.h"

#include "RuntimeReflection/Macro.h"

namespace SteelEngine { namespace Graphics { namespace TestRenderer {

    SE_CLASS(
        IRenderer::RENDERER = "SteelEngine::Graphics::TestRenderer::Renderer"
    )
    class Shader : public IShader
    {
    private:
        void* m_ID;

        const char* m_FragmentSource;
        const char* m_VertexSource;

    public:
        Shader(const char* fSource, const char* vSource);
        ~Shader();

        ShaderUniform* m_Uniforms;

        void Setup(const RenderDevice* renderDevice) override;

        void** GetProgram() const override;
        void* GetLocation(uint32_t index) const override;
    };

}}}