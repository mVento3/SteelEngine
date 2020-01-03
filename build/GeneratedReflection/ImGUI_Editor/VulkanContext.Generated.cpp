#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/ImGUI_Editor/VulkanContext.Generated.h"
#include "ImGUI_Editor/VulkanContext.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

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
.Inheritance<IContext>("IContext")
.Constructor<>()
.Property("m_PushConstBlock", &VulkanContext::m_PushConstBlock)
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
