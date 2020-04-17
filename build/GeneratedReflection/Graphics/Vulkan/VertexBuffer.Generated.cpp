#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Graphics/Vulkan/VertexBuffer.h"
#include "D:\Projects\C++\SteelEngine\build/GeneratedReflection/Graphics/Vulkan/VertexBuffer.Generated.h"

namespace SteelEngine {
namespace Graphics {
namespace Vulkan {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<VertexBuffer>("VertexBuffer",{
"SteelEngine",
"Graphics",
"Vulkan"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(VertexBuffer))
)
.RegisterConstructor<>()
.RegisterInheritance<Buffer>("Buffer")
;
}
}
}
}
