#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Core/DeltaTime.Generated.h"
#include "Core/DeltaTime.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<DeltaTime>("DeltaTime",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(DeltaTime))
)
.Inheritance<IDeltaTime>("IDeltaTime")
.Constructor<>()
;
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(DeltaTime)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
