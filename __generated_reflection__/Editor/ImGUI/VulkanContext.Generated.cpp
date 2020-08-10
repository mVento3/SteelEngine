#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Editor/ImGUI/VulkanContext.h"
#include "Editor/ImGUI/VulkanContext.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<VulkanContext>("VulkanContext",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::HOT_RELOAD, true),
SteelEngine::Reflection::MetaData(Editor::ImGUI::ImGUI_Editor::API::CONTEXT_API , Graphics::IRenderer::API::VULKAN),
SteelEngine::Reflection::MetaData("sizeof", sizeof(VulkanContext))
)
.RegisterConstructor<>()
.RegisterInheritance<IContext>("IContext")
.RegisterMethod("GetContext", &VulkanContext::GetContext)
.RegisterMethod("MakeCurrent", &VulkanContext::MakeCurrent)
.RegisterMethod("Serialize", &VulkanContext::Serialize)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::SERIALIZE_FUNCTION, true)
)
;
}
void VulkanContext::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
}
#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(VulkanContext)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
