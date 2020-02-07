#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Core/ReflectionModules/NaiveEventReflectionModule.h"
#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Core/ReflectionModules/NaiveEventReflectionModule.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<NaiveEventReflectionModule>("NaiveEventReflectionModule",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::REFLECTION_MODULE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(NaiveEventReflectionModule))
)
.Constructor<>()
.Inheritance<IReflectionModule>("IReflectionModule")
;
}
void NaiveEventReflectionModule::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
}
#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(NaiveEventReflectionModule)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
