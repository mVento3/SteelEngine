#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Input/Events/KeyUpEvent.h"
#include "D:\Projects\C++\SteelEngine\__generated_reflection__/Input/Events/KeyUpEvent.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<KeyUpEvent>("KeyUpEvent",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::NAIVE_EVENT, true),
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(KeyUpEvent))
)
.RegisterConstructor<int>()
.RegisterInheritance<Event::Naive>("Event::Naive")
;
}
}
