#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "ImGUI_Editor/VulkanContext.h"
#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/ImGUI_Editor/VulkanContext.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<VulkanContext>("VulkanContext",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(VulkanContext))
)
.Constructor<>()
.Inheritance<IContext>("IContext")
.Method("GetContext", &VulkanContext::GetContext)
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
