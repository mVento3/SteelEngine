#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Core/InformationTracker.Generated.h"
#include "Core/InformationTracker.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<InformationTracker>("InformationTracker",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(InformationTracker))
)
.Inheritance<IInformationTracker>("IInformationTracker")
.Constructor<>()
(
)
;
}

void InformationTracker::Serialize(SteelEngine::HotReload::ISerializer* serializer)
{
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(void* typeInfo)
{
DECLARE_TYPE_INFO(InformationTracker)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
