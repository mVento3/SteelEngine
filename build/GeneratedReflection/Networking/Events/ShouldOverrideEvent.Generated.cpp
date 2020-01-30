#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Networking/Events/ShouldOverrideEvent.Generated.h"
#include "Networking/Events/ShouldOverrideEvent.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

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
.Property("m_File", &ShouldOverrideEvent::m_File)
.Property("m_IsSet", &ShouldOverrideEvent::m_IsSet)
.Property("m_ShouldOverride", &ShouldOverrideEvent::m_ShouldOverride)
.Property("m_ShouldOverrideForAll", &ShouldOverrideEvent::m_ShouldOverrideForAll)
;
}

}
}
