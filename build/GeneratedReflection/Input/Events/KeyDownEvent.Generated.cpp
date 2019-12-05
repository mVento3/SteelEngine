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
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(KeyDownEvent))
)
.Property("m_KeyCode", &KeyDownEvent::m_KeyCode)
;
}

}
