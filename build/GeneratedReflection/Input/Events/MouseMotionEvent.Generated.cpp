#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Input/Events/MouseMotionEvent.h"
#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Input/Events/MouseMotionEvent.Generated.h"

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
#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(MouseMotionEvent)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR(int, int)
};

return result;
}
#endif
}
