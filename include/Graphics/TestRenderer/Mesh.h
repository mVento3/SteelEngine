#pragma once

#include "Graphics/IMesh.h"

#include "Platform/Graphics/RenderDevice.h"

#include "Graphics/IRenderer.h"

namespace SteelEngine { namespace Graphics { namespace TestRenderer {

    SE_CLASS(
        IRenderer::RENDERER = "SteelEngine::Graphics::TestRenderer::Renderer"
    )
    class Mesh : public IMesh
    {
    private:
        void* m_VAO;
        void* m_VBOs;
        uint32_t m_DrawCount;

    public:
        Mesh();
        ~Mesh();

        void Setup(const RenderDevice* renderDevice) override;

        void** GetVAO_() const override { return (void**)&m_VAO; }
        void** GetVBOs_() const override { return (void**)&m_VBOs; }
        void SetDrawCount(uint32_t drawCount) override { m_DrawCount = drawCount; }
        uint32_t GetDrawCount() const override { return m_DrawCount; }
    };

}}}