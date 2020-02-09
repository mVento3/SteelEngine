#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "HotReloader/IRuntimeObject.h"
#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/HotReloader/IRuntimeObject.Generated.h"

namespace SteelEngine {
namespace HotReloader {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<IRuntimeObject>("IRuntimeObject",{
"SteelEngine",
"HotReloader"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(IRuntimeObject))
)
;
}
}
}
