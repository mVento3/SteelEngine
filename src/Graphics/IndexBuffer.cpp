#include "Graphics/IndexBuffer.h"

#include "Utils/Graphics/RenderContext.h"

#include "Graphics/OpenGL/IndexBuffer.h"

namespace SteelEngine { namespace Graphics {

    IndexBuffer* IndexBuffer::Create(Type::uint32* indices, Type::uint32 count)
    {
        switch (SteelEngine::Utils::RenderContext::GetCurrentAPI())
        {
        case IRenderer::API::OPENGL:
            return new OpenGL::IndexBuffer(indices, count);
            break;
        case IRenderer::API::VULKAN:
            return 0;
            break;
        default:
            return 0;
            break;
        }

        return 0;
    }

}}