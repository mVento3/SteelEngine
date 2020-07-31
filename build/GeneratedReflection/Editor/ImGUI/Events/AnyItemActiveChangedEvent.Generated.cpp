#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Editor/ImGUI/Events/AnyItemActiveChangedEvent.h"
#include "D:\Projects\C++\SteelEngine\build/GeneratedReflection/Editor/ImGUI/Events/AnyItemActiveChangedEvent.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<AnyItemActiveChangedEvent>("AnyItemActiveChangedEvent",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::NAIVE_EVENT, true),
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(AnyItemActiveChangedEvent))
)
.RegisterConstructor<bool>()
.RegisterInheritance<Event::Naive>("Event::Naive")
;
}
}
