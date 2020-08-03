#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "RuntimeReflection/ReflectionGenerator.h"
#include "D:\Projects\C++\SteelEngine\__generated_reflection__/RuntimeReflection/ReflectionGenerator.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<ReflectionGenerator>("ReflectionGenerator",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(Reflection::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(ReflectionGenerator))
)
.RegisterConstructor<>()
.RegisterInheritance<IReflectionGenerator>("IReflectionGenerator")
;
}
}
