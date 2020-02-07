#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Window/Events/WindowResizedEvent.h"
#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Window/Events/WindowResizedEvent.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<WindowResizedEvent>("WindowResizedEvent",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::NAIVE_EVENT, true),
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(WindowResizedEvent))
)
.Constructor<Type::uint32, Type::uint32>()
.Inheritance<Event::Naive>("Event::Naive")
;
}
#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(WindowResizedEvent)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR(Type::uint32, Type::uint32)
};

return result;
}
#endif
}
