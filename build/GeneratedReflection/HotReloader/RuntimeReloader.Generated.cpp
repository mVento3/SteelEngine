#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "HotReloader/RuntimeReloader.h"
#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/HotReloader/RuntimeReloader.Generated.h"

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
.Constructor<>()
.Inheritance<IRuntimeReloader>("IRuntimeReloader")
;
}
#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
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
