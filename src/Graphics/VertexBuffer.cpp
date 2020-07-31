#include "Graphics/VertexBuffer.h"

#include "Graphics/OpenGL/VertexBuffer.h"

#include "Utils/Graphics/RenderContext.h"

namespace SteelEngine { namespace Graphics {

    VertexBuffer* VertexBuffer::Create(float* vertices, Type::uint32 count)
    {
        switch (SteelEngine::Utils::RenderContext::GetCurrentAPI())
        {
        case IRenderer::API::OPENGL:
            return new OpenGL::VertexBuffer(vertices, count);
            break;
        case IRenderer::API::VULKAN:
            return 0;
            break;
        default:
            return 0;
            break;
        }
    }

}}