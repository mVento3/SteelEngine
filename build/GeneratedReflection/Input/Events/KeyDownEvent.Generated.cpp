#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Input/Events/KeyDownEvent.Generated.h"
#include "Input/Events/KeyDownEvent.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<KeyDownEvent>("KeyDownEvent",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(KeyDownEvent))
)
.Property("m_KeyCode", &KeyDownEvent::m_KeyCode)
(
)
;
}

}
