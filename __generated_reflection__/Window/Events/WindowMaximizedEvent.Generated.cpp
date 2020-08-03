#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Window/Events/WindowMaximizedEvent.h"
#include "D:\Projects\C++\SteelEngine\__generated_reflection__/Window/Events/WindowMaximizedEvent.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<WindowMaximizedEvent>("WindowMaximizedEvent",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::NAIVE_EVENT, true),
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(WindowMaximizedEvent))
)
.RegisterInheritance<Event::Naive>("Event::Naive")
;
}
}
