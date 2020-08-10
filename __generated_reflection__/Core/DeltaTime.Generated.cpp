#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Core/DeltaTime.h"
#include "Core/DeltaTime.Generated.h"

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
.RegisterConstructor<>()
.RegisterInheritance<IDeltaTime>("IDeltaTime")
;
}
}
