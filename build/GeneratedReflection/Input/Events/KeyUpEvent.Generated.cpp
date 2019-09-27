#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Input/Events/KeyUpEvent.Generated.h"
#include "Input/Events/KeyUpEvent.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<KeyUpEvent>("KeyUpEvent",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(KeyUpEvent))
)
.Property("m_KeyCode", &KeyUpEvent::m_KeyCode)
(
)
;
}

}
