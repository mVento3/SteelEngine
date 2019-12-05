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
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(KeyUpEvent))
)
.Property("m_KeyCode", &KeyUpEvent::m_KeyCode)
;
}

}
