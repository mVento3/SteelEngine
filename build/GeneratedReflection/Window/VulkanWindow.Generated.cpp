#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Window/VulkanWindow.h"
#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Window/VulkanWindow.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<VulkanWindow>("VulkanWindow",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(VulkanWindow))
)
.Constructor<>()
.Inheritance<IWindow>("IWindow")
.Method("SetProcessEventsCallback", &VulkanWindow::SetProcessEventsCallback)
;
}
}
