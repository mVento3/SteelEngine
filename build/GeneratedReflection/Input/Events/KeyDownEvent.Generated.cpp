#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Input/Events/KeyDownEvent.h"
#include "D:\Projects\C++\SteelEngine\build/GeneratedReflection/Input/Events/KeyDownEvent.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<KeyDownEvent>("KeyDownEvent",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::NAIVE_EVENT, true),
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(KeyDownEvent))
)
.RegisterConstructor<int>()
.RegisterInheritance<Event::Naive>("Event::Naive")
;
}
}
