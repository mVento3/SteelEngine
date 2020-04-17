#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Graphics/Vulkan/IndexBuffer.h"
#include "D:\Projects\C++\SteelEngine\build/GeneratedReflection/Graphics/Vulkan/IndexBuffer.Generated.h"

namespace SteelEngine {
namespace Graphics {
namespace Vulkan {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<IndexBuffer>("IndexBuffer",{
"SteelEngine",
"Graphics",
"Vulkan"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(IndexBuffer))
)
.RegisterConstructor<>()
.RegisterInheritance<Buffer>("Buffer")
;
}
}
}
}
