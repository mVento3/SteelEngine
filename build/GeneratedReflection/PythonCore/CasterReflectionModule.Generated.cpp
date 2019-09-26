#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/PythonCore/CasterReflectionModule.Generated.h"
#include "PythonCore/CasterReflectionModule.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<CasterReflectionModule>("CasterReflectionModule",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::REFLECTION_MODULE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(CasterReflectionModule))
)
.Inheritance<HotReload::IRuntimeObject>("HotReload::IRuntimeObject")
.Constructor<>()
(
)
;
}

void CasterReflectionModule::Serialize(SteelEngine::HotReload::ISerializer* serializer)
{
IRuntimeObject::Serialize(serializer);
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(void* typeInfo)
{
DECLARE_TYPE_INFO(CasterReflectionModule)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
