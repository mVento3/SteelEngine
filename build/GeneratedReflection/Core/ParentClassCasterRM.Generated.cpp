#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Core/ParentClassCasterRM.Generated.h"
#include "Core/ParentClassCasterRM.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<ParentClassCasterRM>("ParentClassCasterRM",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::REFLECTION_MODULE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(ParentClassCasterRM))
)
.Inheritance<HotReload::IRuntimeObject>("HotReload::IRuntimeObject")
.Constructor<>()
(
)
;
}

void ParentClassCasterRM::Serialize(SteelEngine::HotReload::ISerializer* serializer)
{
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(void* typeInfo)
{
DECLARE_TYPE_INFO(ParentClassCasterRM)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
