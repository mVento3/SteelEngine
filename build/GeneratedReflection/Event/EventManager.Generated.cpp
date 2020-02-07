#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Event/EventManager.h"
#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Event/EventManager.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<EventManager>("EventManager",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(EventManager))
)
.Constructor<>()
.Inheritance<IEventManager>("IEventManager")
;
}
void EventManager::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
}
#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(EventManager)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
