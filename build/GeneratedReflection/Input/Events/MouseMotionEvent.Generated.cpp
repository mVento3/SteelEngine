#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Input/Events/MouseMotionEvent.h"
#include "D:\Projects\C++\SteelEngine/build/GeneratedReflection/Input/Events/MouseMotionEvent.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<MouseMotionEvent>("MouseMotionEvent",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::NAIVE_EVENT, true),
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(MouseMotionEvent))
)
.Constructor<int, int>()
.Inheritance<Event::Naive>("Event::Naive")
;
}
}
