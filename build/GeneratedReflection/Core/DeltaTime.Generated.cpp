#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Core/DeltaTime.h"
#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Core/DeltaTime.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<DeltaTime>("DeltaTime",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(DeltaTime))
)
.Constructor<>()
.Inheritance<IDeltaTime>("IDeltaTime")
;
}
}
