#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Core/GetCompileConfigEvent.Generated.h"
#include "Core/GetCompileConfigEvent.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<GetCompileConfigEvent>("GetCompileConfigEvent",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(GetCompileConfigEvent))
)
.Property("m_Config", &GetCompileConfigEvent::m_Config)
(
)
;
}

}
