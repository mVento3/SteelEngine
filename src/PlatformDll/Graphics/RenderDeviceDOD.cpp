#include "PlatformDll/Graphics/RenderDeviceDOD.h"

#include "PlatformDll/Graphics/Vulkan/VulkanRenderDevice.h"

#include "PlatformDll/Graphics/OpenGL/OpenGL_RenderDevice.h"

SteelEngine::Platform::Graphics::RenderDeviceDOD* render_device_construct(SteelEngine::Graphics::IRenderer::API api)
{
    switch(api)
    {
    case SteelEngine::Graphics::IRenderer::API::VULKAN:
        return SteelEngine::Platform::Vulkan::render_device_construct();
        break;
    case SteelEngine::Graphics::IRenderer::API::OPENGL:
        return SteelEngine::Platform::OpenGL::render_device_construct();
        break;
    default:
        break;
    }
}