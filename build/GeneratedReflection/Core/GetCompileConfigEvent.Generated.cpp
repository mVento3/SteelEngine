#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Core/GetCompileConfigEvent.h"
#include "D:\Projects\C++\SteelEngine\build/GeneratedReflection/Core/GetCompileConfigEvent.Generated.h"

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
;
}
}
