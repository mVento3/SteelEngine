#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Core/PythonScriptableInheritance.h"
#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Core/PythonScriptableInheritance.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<PythonScriptableInheritance>("PythonScriptableInheritance",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::INHERITANCE_MODULE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(PythonScriptableInheritance))
)
.Constructor<>()
.Inheritance<HotReloader::IRuntimeObject>("HotReloader::IRuntimeObject")
.Method("ProcessInheritance", &PythonScriptableInheritance::ProcessInheritance)
;
}
void PythonScriptableInheritance::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
}
#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(PythonScriptableInheritance)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
