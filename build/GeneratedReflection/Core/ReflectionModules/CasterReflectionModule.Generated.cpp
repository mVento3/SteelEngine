#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Core/ReflectionModules/CasterReflectionModule.h"
#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Core/ReflectionModules/CasterReflectionModule.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<CasterReflectionModule>("CasterReflectionModule",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::REFLECTION_MODULE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(CasterReflectionModule))
)
.Constructor<>()
.Inheritance<IReflectionModule>("IReflectionModule")
;
}
void CasterReflectionModule::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
}
#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
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
