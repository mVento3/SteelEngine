#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Graphics/Vulkan/Buffer.h"
#include "D:\Projects\C++\SteelEngine/build/GeneratedReflection/Graphics/Vulkan/Buffer.Generated.h"

namespace SteelEngine {
namespace Graphics {
namespace Vulkan {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<Buffer>("Buffer",{
"SteelEngine",
"Graphics",
"Vulkan"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(Buffer))
)
.Constructor<>()
.Inheritance<IBuffer>("IBuffer")
;
}
}
}
}
