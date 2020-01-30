#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Window/Events/WindowRestoredEvent.Generated.h"
#include "Window/Events/WindowRestoredEvent.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<WindowRestoredEvent>("WindowRestoredEvent",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::NAIVE_EVENT, true),
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(WindowRestoredEvent))
)
.Inheritance<Event::Naive>("Event::Naive")
;
}

}
