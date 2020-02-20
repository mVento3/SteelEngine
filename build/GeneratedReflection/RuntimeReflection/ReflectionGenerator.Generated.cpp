#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "RuntimeReflection/ReflectionGenerator.h"
#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/RuntimeReflection/ReflectionGenerator.Generated.h"

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
.Constructor<>()
.Inheritance<IReflectionGenerator>("IReflectionGenerator")
;
}
}
