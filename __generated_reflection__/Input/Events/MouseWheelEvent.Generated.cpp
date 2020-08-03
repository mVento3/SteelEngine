#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Input/Events/MouseWheelEvent.h"
#include "D:\Projects\C++\SteelEngine\__generated_reflection__/Input/Events/MouseWheelEvent.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<MouseWheelEvent>("MouseWheelEvent",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::NAIVE_EVENT, true),
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(MouseWheelEvent))
)
.RegisterConstructor<float>()
.RegisterInheritance<Event::Naive>("Event::Naive")
;
}
}
