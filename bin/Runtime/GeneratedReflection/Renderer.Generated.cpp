#include "Renderer.Generated.h"
#include "../include\Rendering\Vulkan\Renderer.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<SteelEngine::Graphics::Vulkan::Renderer>("Renderer")
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::SE_RUNTIME_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(SteelEngine::Graphics::Vulkan::Renderer))
)
.Inheritance("Interface::IRenderer")
.Constructor<SteelEngine::Interface::IWindow*>()
(
)
.Method("RecreateSwapChain", &SteelEngine::Graphics::Vulkan::Renderer::RecreateSwapChain)
(
)
;
}

void SteelEngine::Graphics::Vulkan::Renderer::Serialize(SteelEngine::Interface::ISerializer* serializer)
{
SERIALIZE(SteelEngine::Graphics::Vulkan::Renderer::m_Instance)
SERIALIZE(SteelEngine::Graphics::Vulkan::Renderer::m_DebugMessenger)
SERIALIZE(SteelEngine::Graphics::Vulkan::Renderer::m_Window)
SERIALIZE(SteelEngine::Graphics::Vulkan::Renderer::m_PhysicalDevice)
SERIALIZE(SteelEngine::Graphics::Vulkan::Renderer::m_LogicalDevice)
SERIALIZE(SteelEngine::Graphics::Vulkan::Renderer::m_Surface)
SERIALIZE(SteelEngine::Graphics::Vulkan::Renderer::m_SwapChain)
SERIALIZE(SteelEngine::Graphics::Vulkan::Renderer::m_GraphicsPipeline)
SERIALIZE(SteelEngine::Graphics::Vulkan::Renderer::m_RenderPass)
SERIALIZE(SteelEngine::Graphics::Vulkan::Renderer::m_Framebuffer)
SERIALIZE(SteelEngine::Graphics::Vulkan::Renderer::m_CommandPool)
SERIALIZE(SteelEngine::Graphics::Vulkan::Renderer::m_DescriptorPool)
SERIALIZE(SteelEngine::Graphics::Vulkan::Renderer::m_Descriptor)
SERIALIZE(SteelEngine::Graphics::Vulkan::Renderer::m_ImageAvailableSemaphores)
SERIALIZE(SteelEngine::Graphics::Vulkan::Renderer::m_RenderFinishedSemaphores)
SERIALIZE(SteelEngine::Graphics::Vulkan::Renderer::m_InFlightFences)
SERIALIZE(SteelEngine::Graphics::Vulkan::Renderer::m_ShaderStages)
SERIALIZE(SteelEngine::Graphics::Vulkan::Renderer::m_CurrentFrame)
SERIALIZE(SteelEngine::Graphics::Vulkan::Renderer::m_FramebufferResized)
SERIALIZE(SteelEngine::Graphics::Vulkan::Renderer::m_WindowMinimized)
SERIALIZE(SteelEngine::Graphics::Vulkan::Renderer::m_Width)
SERIALIZE(SteelEngine::Graphics::Vulkan::Renderer::m_Height)
SERIALIZE(SteelEngine::Graphics::Vulkan::Renderer::m_SomeShader)
SERIALIZE(SteelEngine::Graphics::Vulkan::Renderer::m_Buffer)
SERIALIZE(SteelEngine::Graphics::Vulkan::Renderer::m_Buffer2)
SERIALIZE(SteelEngine::Graphics::Vulkan::Renderer::m_IndexBuffer)
SERIALIZE(SteelEngine::Graphics::Vulkan::Renderer::m_VertexArray)
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* GetPerModuleInterface(void* typeInfo)
{
DECLARE_TYPE_INFO(SteelEngine::Graphics::Vulkan::Renderer)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR(SteelEngine::Interface::IWindow*)
};

return result;
}
#endif
