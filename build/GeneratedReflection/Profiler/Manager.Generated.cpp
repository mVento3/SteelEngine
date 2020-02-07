#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Profiler/Manager.h"
#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Profiler/Manager.Generated.h"

namespace SteelEngine {
namespace Profiler {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<Manager>("Manager",{
"SteelEngine",
"Profiler"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(Manager))
)
.Constructor<>()
.Inheritance<IManager>("IManager")
.Method("AddTime", &Manager::AddTime)
.Method("GetTime", &Manager::GetTime)
;
}
void Manager::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
}
#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(Manager)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
}
