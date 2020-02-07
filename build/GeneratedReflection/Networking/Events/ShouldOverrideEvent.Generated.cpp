#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Networking/Events/ShouldOverrideEvent.h"
#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Networking/Events/ShouldOverrideEvent.Generated.h"

namespace SteelEngine {
namespace Network {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<ShouldOverrideEvent>("ShouldOverrideEvent",{
"SteelEngine",
"Network"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(ShouldOverrideEvent))
)
;
}
}
}
