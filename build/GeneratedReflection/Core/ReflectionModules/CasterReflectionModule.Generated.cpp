#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Core/ReflectionModules/CasterReflectionModule.Generated.h"
#include "Core/ReflectionModules/CasterReflectionModule.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<CasterReflectionModule>("CasterReflectionModule",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::REFLECTION_MODULE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(CasterReflectionModule))
)
.Inheritance<HotReloader::IRuntimeObject>("HotReloader::IRuntimeObject")
.Constructor<>()
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
