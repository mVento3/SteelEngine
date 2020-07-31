#pragma once

#include "Graphics/IRenderer.h"
#include "Graphics/IMesh.h"

#include "Utils/Graphics/RenderContext.h"

// #include "Graphics/VertexBuffer.h"
// #include "Graphics/IndexBuffer.h"

namespace SteelEngine { namespace Graphics { namespace Utils {

    class Renderer3D
    {
    private:
        static RuntimeDatabase* sm_RuntimeDatabase;
        static SteelEngine::Utils::RenderContext* sm_Context;

    public:
        Renderer3D();
        ~Renderer3D();

    // Add model to render queue
        static void AddModel(IModel* model, const Transform& transform, bool castShadow = true);

    // Should be called in module where will be used
        static void InitializeInCurrentModule();

    // API
        static void Draw(float* vertices, size_t vSize, uint32_t* indices, size_t iSize);

        static void Draw(SteelEngine::Graphics::IMesh* mesh);

        // static inline API GetCurrentAPI() { return sm_CurrentAPI; }
    };

}}}