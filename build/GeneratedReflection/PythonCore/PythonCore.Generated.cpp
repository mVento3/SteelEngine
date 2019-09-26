#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/PythonCore/PythonCore.Generated.h"
#include "PythonCore/PythonCore.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {
namespace Script {
REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<PythonCore>("PythonCore",{
"SteelEngine",
"Script"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(PythonCore))
)
.Inheritance<IPython>("IPython")
.Constructor<>()
(
)
;
}

void PythonCore::Serialize(SteelEngine::HotReload::ISerializer* serializer)
{
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(void* typeInfo)
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
