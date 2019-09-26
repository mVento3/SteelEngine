#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Core/NCE_ReflectionModule.Generated.h"
#include "Core/NCE_ReflectionModule.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<NCE_ReflectionModule>("NCE_ReflectionModule",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::REFLECTION_MODULE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(NCE_ReflectionModule))
)
.Inheritance<HotReload::IRuntimeObject>("HotReload::IRuntimeObject")
.Constructor<>()
(
)
;
}

void NCE_ReflectionModule::Serialize(SteelEngine::HotReload::ISerializer* serializer)
{
IRuntimeObject::Serialize(serializer);
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(void* typeInfo)
{
DECLARE_TYPE_INFO(NCE_ReflectionModule)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
