#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "HotReloader/RuntimeReloader.h"
#include "D:\Projects\C++\SteelEngine/build/GeneratedReflection/HotReloader/RuntimeReloader.Generated.h"

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
.Constructor<>()
.Inheritance<IRuntimeReloader>("IRuntimeReloader")
;
}
}
}
