#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Graphics/Vulkan/Renderer.Generated.h"
#include "Graphics/Vulkan/Renderer.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

namespace SteelEngine {
namespace Graphics {
namespace Vulkan {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<Renderer>("Renderer",{
"SteelEngine",
"Graphics",
"Vulkan"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::RUNTIME_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(Renderer))
)
.Inheritance<IRendererAPI<IRenderer::API::VULKAN_API>>("IRendererAPI<IRenderer::API::VULKAN_API>")
.Inheritance<EventObserver>("EventObserver")
.Constructor<IWindow*>()
.Method("RecreateSwapChain", &Renderer::RecreateSwapChain)
;
}

void Renderer::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
SERIALIZE(Renderer::m_Instance)
SERIALIZE(Renderer::m_Device)
SERIALIZE(Renderer::m_Validation)
SERIALIZE(Renderer::m_CommandPool)
SERIALIZE(Renderer::m_CommandBuffer)
SERIALIZE(Renderer::m_DescriptorPool)
SERIALIZE(Renderer::m_Surface)
SERIALIZE(Renderer::m_SwapChain)
SERIALIZE(Renderer::m_Camera)
SERIALIZE(Renderer::m_RenderPass)
SERIALIZE(Renderer::m_DescriptorSetLayout)
SERIALIZE(Renderer::m_PipelineLayout)
SERIALIZE(Renderer::m_Pipeline)
SERIALIZE(Renderer::m_PipelineCache)
SERIALIZE(Renderer::m_Framebuffers)
SERIALIZE(Renderer::m_CommandBuffers)
SERIALIZE(Renderer::m_Window)
SERIALIZE(Renderer::m_ImageAvailableSemaphores)
SERIALIZE(Renderer::m_RenderFinishedSemaphores)
SERIALIZE(Renderer::m_InFlightFences)
SERIALIZE(Renderer::m_ShaderStages)
SERIALIZE(Renderer::m_Commands)
SERIALIZE(Renderer::m_UpdateBuffers)
SERIALIZE(Renderer::m_CurrentFrame)
SERIALIZE(Renderer::m_FramebufferResized)
SERIALIZE(Renderer::m_WindowMinimized)
SERIALIZE(Renderer::m_Keys)
SERIALIZE(Renderer::m_RotateCamera)
SERIALIZE(Renderer::m_Width)
SERIALIZE(Renderer::m_Height)
SERIALIZE(Renderer::m_SomeShader)
SERIALIZE(Renderer::m_Buffer)
SERIALIZE(Renderer::m_Buffer2)
SERIALIZE(Renderer::m_IndexBuffer)
SERIALIZE(Renderer::m_MVP_Uniform)
SERIALIZE(Renderer::m_MVP_Uniform2)
SERIALIZE(Renderer::m_Trans)
SERIALIZE(Renderer::m_Trans2)
SERIALIZE(Renderer::m_VertexArray)
SERIALIZE(Renderer::m_DeltaTimeVariant)
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(Renderer)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR(IWindow*)
};

return result;
}
#endif
}
}
}
