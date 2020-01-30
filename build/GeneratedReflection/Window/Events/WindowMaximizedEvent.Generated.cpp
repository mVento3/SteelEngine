#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Window/Events/WindowMaximizedEvent.Generated.h"
#include "Window/Events/WindowMaximizedEvent.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

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
.Inheritance<Event::Naive>("Event::Naive")
;
}

}
