#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Profiler/Manager.h"
#include "Profiler/Manager.Generated.h"

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
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::HOT_RELOAD, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(Manager))
)
.RegisterConstructor<>()
.RegisterInheritance<IManager>("IManager")
.RegisterMethod("AddTime", { SteelEngine::Reflection::FucntionArgument<Manager::TimeData>("time") }, &Manager::AddTime)
.RegisterMethod("GetTime", &Manager::GetTime)
.RegisterMethod("Serialize", &Manager::Serialize)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::SERIALIZE_FUNCTION, true)
)
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
