#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/HotReloader/RuntimeReloader.Generated.h"
#include "HotReloader/RuntimeReloader.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

namespace SteelEngine {
namespace HotReloader {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<RuntimeReloader>("RuntimeReloader",{
"SteelEngine",
"HotReloader"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(RuntimeReloader))
)
.Inheritance<IRuntimeReloader>("IRuntimeReloader")
.Constructor<>()
;
}

void RuntimeReloader::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(RuntimeReloader)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
}
