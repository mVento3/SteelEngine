#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "RuntimeReflection/ReflectionGenerator.h"
#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/RuntimeReflection/ReflectionGenerator.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<ReflectionGenerator>("ReflectionGenerator",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(Reflection::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(ReflectionGenerator))
)
.Constructor<>()
.Inheritance<IReflectionGenerator>("IReflectionGenerator")
;
}
#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(ReflectionGenerator)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
