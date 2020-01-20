#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/ImGUI_Editor/Events/AnyItemActiveChangedEvent.Generated.h"
#include "ImGUI_Editor/Events/AnyItemActiveChangedEvent.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<AnyItemActiveChangedEvent>("AnyItemActiveChangedEvent",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::NAIVE_EVENT, true),
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(AnyItemActiveChangedEvent))
)
.Inheritance<Event::Naive>("Event::Naive")
.Constructor<bool>()
.Constructor<bool>()
.Property("m_State", &AnyItemActiveChangedEvent::m_State)
;
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(AnyItemActiveChangedEvent)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR(bool)
COMPARE_CONSTRUCTOR(bool)
};

return result;
}
#endif
}
