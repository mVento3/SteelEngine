#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Graphics/Vulkan/Buffer.Generated.h"
#include "Graphics/Vulkan/Buffer.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

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
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(Buffer))
)
.Inheritance<IBuffer>("IBuffer")
.Constructor<>()
;
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(Buffer)
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
