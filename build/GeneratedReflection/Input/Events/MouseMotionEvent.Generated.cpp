#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Input/Events/MouseMotionEvent.Generated.h"
#include "Input/Events/MouseMotionEvent.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<MouseMotionEvent>("MouseMotionEvent",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(MouseMotionEvent))
)
.Property("m_X", &MouseMotionEvent::m_X)
(
)
.Property("m_Y", &MouseMotionEvent::m_Y)
(
)
;
}

}
