#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Input/Events/MouseMotionEvent.Generated.h"
#include "Input/Events/MouseMotionEvent.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

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
.Inheritance<Event::Naive>("Event::Naive")
.Constructor<int, int>()
.Constructor<int, int>()
.Property("m_PositionX", &MouseMotionEvent::m_PositionX)
.Property("m_PositionY", &MouseMotionEvent::m_PositionY)
;
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(MouseMotionEvent)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR(int, int)
COMPARE_CONSTRUCTOR(int, int)
};

return result;
}
#endif
}
