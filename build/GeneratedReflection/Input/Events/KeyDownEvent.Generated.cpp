#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Input/Events/KeyDownEvent.h"
#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Input/Events/KeyDownEvent.Generated.h"

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
.Constructor<int>()
.Inheritance<Event::Naive>("Event::Naive")
;
}
#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(KeyDownEvent)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR(int)
};

return result;
}
#endif
}
