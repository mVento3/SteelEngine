#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/PythonCore/PythonCore.Generated.h"
#include "PythonCore/PythonCore.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

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
SteelEngine::Reflection::MetaData("sizeof", sizeof(PythonCore))
)
.Inheritance<IPython>("IPython")
.Constructor<>()
;
}

void PythonCore::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(RuntimeDatabase::ConstructedObjectsVector* typeInfo)
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
