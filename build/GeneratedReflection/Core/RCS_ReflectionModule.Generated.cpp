#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Core/RCS_ReflectionModule.Generated.h"
#include "Core/RCS_ReflectionModule.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<RCS_ReflectionModule>("RCS_ReflectionModule",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::REFLECTION_MODULE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(RCS_ReflectionModule))
)
.Inheritance<HotReload::IRuntimeObject>("HotReload::IRuntimeObject")
.Constructor<>()
(
)
;
}

void RCS_ReflectionModule::Serialize(SteelEngine::HotReload::ISerializer* serializer)
{
IRuntimeObject::Serialize(serializer);
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(void* typeInfo)
{
DECLARE_TYPE_INFO(RCS_ReflectionModule)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
