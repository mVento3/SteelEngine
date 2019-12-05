#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/RuntimeCompiler/RecompiledEvent.Generated.h"
#include "HotReloader/RecompiledEvent.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<RecompiledEvent>("RecompiledEvent",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(RecompiledEvent))
)
.Property("m_TypeID", &RecompiledEvent::m_TypeID)
(
)
.Property("m_NewObject", &RecompiledEvent::m_NewObject)
(
)
.Property("m_OldObject", &RecompiledEvent::m_OldObject)
(
)
;
}

}
