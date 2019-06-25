#include "VulkanWindow.Generated.h"
#include "../include\Window\VulkanWindow.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<SteelEngine::VulkanWindow>("VulkanWindow")
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::SE_NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(SteelEngine::VulkanWindow))
)
.Inheritance("Interface::IWindow")
.Constructor<>()
(
)
.Method("SetProcessEventsCallback", &SteelEngine::VulkanWindow::SetProcessEventsCallback)
(
)
;
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* GetPerModuleInterface(void* typeInfo)
{
DECLARE_TYPE_INFO(SteelEngine::VulkanWindow)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
