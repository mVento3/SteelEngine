#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Window/VulkanWindow.Generated.h"
#include "Window/VulkanWindow.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<VulkanWindow>("VulkanWindow",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(VulkanWindow))
)
.Inheritance<IWindow>("IWindow")
.Constructor<>()
.Method("SetProcessEventsCallback", &VulkanWindow::SetProcessEventsCallback)
;
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(VulkanWindow)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
