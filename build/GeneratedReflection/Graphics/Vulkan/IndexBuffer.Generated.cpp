#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Graphics/Vulkan/IndexBuffer.Generated.h"
#include "Graphics/Vulkan/IndexBuffer.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {
namespace Graphics {
namespace Vulkan {
REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<IndexBuffer>("IndexBuffer",{
"SteelEngine",
"Graphics",
"Vulkan"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(IndexBuffer))
)
.Inheritance<Buffer>("Buffer")
.Constructor<>()
(
)
;
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(void* typeInfo)
{
DECLARE_TYPE_INFO(IndexBuffer)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
}
}
