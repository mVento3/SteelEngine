#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Core/PythonScriptableInheritance.Generated.h"
#include "Core/PythonScriptableInheritance.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<PythonScriptableInheritance>("PythonScriptableInheritance",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::INHERITANCE_MODULE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(PythonScriptableInheritance))
)
.Inheritance<HotReloader::IRuntimeObject>("HotReloader::IRuntimeObject")
.Constructor<>()
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
