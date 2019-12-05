#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Input/Events/ChangeMousePositionEvent.Generated.h"
#include "Input/Events/ChangeMousePositionEvent.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<ChangeMousePositionEvent>("ChangeMousePositionEvent",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(ChangeMousePositionEvent))
)
.Property("m_X", &ChangeMousePositionEvent::m_X)
.Property("m_Y", &ChangeMousePositionEvent::m_Y)
;
}

}
