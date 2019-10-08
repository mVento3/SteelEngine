#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Graphics/Vulkan/Shader.Generated.h"
#include "Graphics/Vulkan/Shader.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {
namespace Graphics {
namespace Vulkan {
REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<Shader>("Shader",{
"SteelEngine",
"Graphics",
"Vulkan"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(Shader))
)
.Inheritance<IShader>("IShader")
.Constructor<>()
(
)
;
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(void* typeInfo)
{
DECLARE_TYPE_INFO(Shader)
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
