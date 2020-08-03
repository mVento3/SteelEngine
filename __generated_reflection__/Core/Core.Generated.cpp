#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Core/Core.h"
#include "D:\Projects\C++\SteelEngine\__generated_reflection__/Core/Core.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<Core>("Core",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(Core))
)
.RegisterConstructor<>()
.RegisterInheritance<ICore>("ICore")
.RegisterMethod("Start", &Core::Start)
.RegisterMethod("Stop", &Core::Stop)
.RegisterMethod("SetPathVariant", { SteelEngine::Reflection::FucntionArgument<Core::EnginePathVariant>("variant") }, &Core::SetPathVariant)
;
}
}
