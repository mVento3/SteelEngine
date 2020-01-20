#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Input/Events/KeyUpEvent.Generated.h"
#include "Input/Events/KeyUpEvent.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<KeyUpEvent>("KeyUpEvent",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::NAIVE_EVENT, true),
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(KeyUpEvent))
)
.Inheritance<Event::Naive>("Event::Naive")
.Constructor<int>()
.Constructor<int>()
.Property("m_KeyCode", &KeyUpEvent::m_KeyCode)
;
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(KeyUpEvent)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR(int)
COMPARE_CONSTRUCTOR(int)
};

return result;
}
#endif
}
