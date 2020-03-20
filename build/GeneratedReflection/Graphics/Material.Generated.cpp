#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Graphics/Material.h"
#include "D:\Projects\C++\SteelEngine/build/GeneratedReflection/Graphics/Material.Generated.h"

namespace SteelEngine {
namespace Graphics {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<Material>("Material",{
"SteelEngine",
"Graphics"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(Material))
)
.Constructor<const char*>()
.Inheritance<IMaterial>("IMaterial")
;
}
}
}
