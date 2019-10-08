#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/ImGUI_Editor/VulkanContext.Generated.h"
#include "ImGUI_Editor/VulkanContext.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<VulkanContext>("VulkanContext",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(VulkanContext))
)
.Inheritance<IContext>("IContext")
.Constructor<>()
(
)
.Property("m_PushConstBlock", &VulkanContext::m_PushConstBlock)
(
)
.Method("GetContext", &VulkanContext::GetContext)
(
)
;
}

void VulkanContext::Serialize(SteelEngine::HotReload::ISerializer* serializer)
{
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(void* typeInfo)
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
