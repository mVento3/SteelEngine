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
}
