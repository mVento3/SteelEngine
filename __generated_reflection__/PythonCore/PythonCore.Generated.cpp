#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "PythonCore/PythonCore.h"
#include "D:\Projects\C++\SteelEngine\__generated_reflection__/PythonCore/PythonCore.Generated.h"

namespace SteelEngine {
namespace Script {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<PythonCore>("PythonCore",{
"SteelEngine",
"Script"
}
)
(
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::HOT_RELOAD, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(PythonCore))
)
.RegisterConstructor<>()
.RegisterInheritance<IPython>("IPython")
.RegisterMethod("Serialize", &PythonCore::Serialize)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::SERIALIZE_FUNCTION, true)
)
;
}
void PythonCore::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
}
#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(PythonCore)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
}
