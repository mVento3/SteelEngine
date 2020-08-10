#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "HotReloader/RuntimeReloader.h"
#include "HotReloader/RuntimeReloader.Generated.h"

namespace SteelEngine {
namespace HotReloader {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<RuntimeReloader>("RuntimeReloader",{
"SteelEngine",
"HotReloader"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(RuntimeReloader))
)
.RegisterConstructor<>()
.RegisterInheritance<IRuntimeReloader>("IRuntimeReloader")
;
}
}
}
