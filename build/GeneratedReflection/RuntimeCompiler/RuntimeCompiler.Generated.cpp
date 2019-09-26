#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/RuntimeCompiler/RuntimeCompiler.Generated.h"
#include "RuntimeCompiler/RuntimeCompiler.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {
namespace HotReload {
REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<RuntimeCompiler>("RuntimeCompiler",{
"SteelEngine",
"HotReload"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(RuntimeCompiler))
)
.Inheritance<IRuntimeCompiler>("IRuntimeCompiler")
.Constructor<>()
(
)
;
}

void RuntimeCompiler::Serialize(SteelEngine::HotReload::ISerializer* serializer)
{
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(void* typeInfo)
{
DECLARE_TYPE_INFO(RuntimeCompiler)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
}
