#include "VulkanRenderer.Generated.h"
#include "../include\Rendering\VulkanRenderer.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<SteelEngine::VulkanRenderer>("VulkanRenderer")
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(SteelEngine::VulkanRenderer))
)
.Inheritance("Interface::IRenderer")
.Constructor<SteelEngine::Interface::IWindow*>()
(
)
;
}

void SteelEngine::VulkanRenderer::Serialize(SteelEngine::Interface::ISerializer* serializer)
{
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* GetPerModuleInterface(void* typeInfo)
{
DECLARE_TYPE_INFO(SteelEngine::VulkanRenderer)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR(SteelEngine::Interface::IWindow*)
};

return result;
}
#endif
