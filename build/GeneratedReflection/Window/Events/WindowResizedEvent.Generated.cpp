#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Window/Events/WindowResizedEvent.h"
#include "D:\Projects\C++\SteelEngine\build/GeneratedReflection/Window/Events/WindowResizedEvent.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<WindowResizedEvent>("WindowResizedEvent",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::NAIVE_EVENT, true),
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(WindowResizedEvent))
)
.RegisterConstructor<Type::uint32, Type::uint32>()
.RegisterInheritance<Event::Naive>("Event::Naive")
;
}
}
