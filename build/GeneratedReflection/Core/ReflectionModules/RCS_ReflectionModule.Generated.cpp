#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Core/ReflectionModules/RCS_ReflectionModule.h"
#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Core/ReflectionModules/RCS_ReflectionModule.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<RCS_ReflectionModule>("RCS_ReflectionModule",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::REFLECTION_MODULE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(RCS_ReflectionModule))
)
.Constructor<>()
.Inheritance<IReflectionModule>("IReflectionModule")
;
}
void RCS_ReflectionModule::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
}
#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
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
