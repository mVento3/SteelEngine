#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Input/Events/ChangeMousePositionEvent.Generated.h"
#include "Input/Events/ChangeMousePositionEvent.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<ChangeMousePositionEvent>("ChangeMousePositionEvent",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(ChangeMousePositionEvent))
)
.Property("m_X", &ChangeMousePositionEvent::m_X)
(
)
.Property("m_Y", &ChangeMousePositionEvent::m_Y)
(
)
;
}

}
