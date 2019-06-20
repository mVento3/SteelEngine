#include "Renderer.Generated.h"
#include "../include\Rendering\Vulkan\Renderer.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<SteelEngine::Graphics::Vulkan::Renderer>("Renderer")
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(SteelEngine::Graphics::Vulkan::Renderer))
)
.Inheritance("Interface::IRenderer")
.Constructor<SteelEngine::Interface::IWindow*>()
(
)
;
}

void SteelEngine::Graphics::Vulkan::Renderer::Serialize(SteelEngine::Interface::ISerializer* serializer)
{
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
