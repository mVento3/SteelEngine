#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Graphics/Vulkan/Shader.h"
#include "D:\Projects\C++\SteelEngine/build/GeneratedReflection/Graphics/Vulkan/Shader.Generated.h"

namespace SteelEngine {
namespace Graphics {
namespace Vulkan {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<Shader>("Shader",{
"SteelEngine",
"Graphics",
"Vulkan"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(Shader))
)
.Constructor<>()
.Inheritance<IShader>("IShader")
;
}
}
}
}
