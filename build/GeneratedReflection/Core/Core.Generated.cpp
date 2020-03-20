#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Core/Core.h"
#include "D:\Projects\C++\SteelEngine/build/GeneratedReflection/Core/Core.Generated.h"

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
.Constructor<>()
.Inheritance<ICore>("ICore")
.Method("Start", &Core::Start)
.Method("Stop", &Core::Stop)
.Method("SetPathVariant", { SteelEngine::Reflection::FucntionArgument<Core::EnginePathVariant>("variant") }, &Core::SetPathVariant)
;
}
}
