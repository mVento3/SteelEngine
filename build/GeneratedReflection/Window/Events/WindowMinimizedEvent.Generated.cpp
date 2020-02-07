#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Window/Events/WindowMinimizedEvent.h"
#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Window/Events/WindowMinimizedEvent.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<WindowMinimizedEvent>("WindowMinimizedEvent",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::NAIVE_EVENT, true),
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(WindowMinimizedEvent))
)
.Inheritance<Event::Naive>("Event::Naive")
;
}
}
