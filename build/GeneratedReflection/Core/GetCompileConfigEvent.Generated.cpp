#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Core/GetCompileConfigEvent.Generated.h"
#include "Core/GetCompileConfigEvent.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<GetCompileConfigEvent>("GetCompileConfigEvent",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(GetCompileConfigEvent))
)
.Property("m_Config", &GetCompileConfigEvent::m_Config)
;
}

}
