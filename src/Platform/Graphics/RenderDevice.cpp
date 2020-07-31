#include "Platform/Graphics/RenderDevice.h"

#include "Platform/Graphics/OpenGL/OpenGL.h"
#include "Platform/Graphics/Vulkan/Vulkan.h"

#include "Utils/Graphics/RenderContext.h"

namespace SteelEngine { namespace Graphics {

    using RenderContext = SteelEngine::Utils::RenderContext;

    RenderDevice::RenderDevice()
    {

    }

    RenderDevice::~RenderDevice()
    {

    }

    RenderDevice* RenderDevice::Create()
    {
        switch(RenderContext::GetCurrentAPI())
        {
        case Graphics::IRenderer::API::OPENGL:
            return new OpenGL();
        case Graphics::IRenderer::API::VULKAN:
            return new Vulkan();
        default:
            break;
        }

        return 0;
    }

}}