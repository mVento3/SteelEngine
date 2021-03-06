#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "RuntimeCompiler/RuntimeCompiler.h"
#include "RuntimeCompiler/RuntimeCompiler.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<RuntimeCompiler>("RuntimeCompiler",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(RuntimeCompiler))
)
.RegisterConstructor<>()
.RegisterConstructor<ISubprocess*>()
.RegisterInheritance<IRuntimeCompiler>("IRuntimeCompiler")
;
}
}
