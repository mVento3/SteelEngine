#pragma once

#include "Graphics/IRenderer.h"

namespace SteelEngine { namespace Graphics {

    class Renderer
    {
    private:
        static const IRenderer* m_CurrentRenderer;

    public:
        Renderer();
        ~Renderer();

        // Should be called in module where will be used
        static void InitializeInCurrentModule()
        {
            // Get from runtime database
        }

        static void BindBuffer()
        {
            // m_CurrentRenderer->
        }
    };

}}