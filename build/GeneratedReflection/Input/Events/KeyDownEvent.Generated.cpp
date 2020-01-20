#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Input/Events/KeyDownEvent.Generated.h"
#include "Input/Events/KeyDownEvent.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<KeyDownEvent>("KeyDownEvent",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::NAIVE_EVENT, true),
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(KeyDownEvent))
)
.Inheritance<Event::Naive>("Event::Naive")
.Constructor<int>()
.Constructor<int>()
.Property("m_KeyCode", &KeyDownEvent::m_KeyCode)
;
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(KeyDownEvent)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR(int)
COMPARE_CONSTRUCTOR(int)
};

return result;
}
#endif
}
