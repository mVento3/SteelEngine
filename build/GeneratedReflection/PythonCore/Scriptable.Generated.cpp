#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/PythonCore/Scriptable.Generated.h"
#include "PythonCore/Scriptable.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {
namespace Script {
namespace Python {
REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<Scriptable>("Scriptable",{
"SteelEngine",
"Script",
"Python"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(Scriptable))
)
.Constructor<const std::string&>()
(
)
.Method("SetPython", &Scriptable::SetPython)
(
)
.Method("GetScriptName", &Scriptable::GetScriptName)
(
)
;
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(void* typeInfo)
{
DECLARE_TYPE_INFO(Scriptable)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR(const std::string&)
};

return result;
}
#endif
}
}
}
