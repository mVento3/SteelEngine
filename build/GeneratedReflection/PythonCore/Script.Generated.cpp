#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/PythonCore/Script.Generated.h"
#include "PythonCore/Script.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

namespace SteelEngine {
namespace Script {
namespace Python {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<Script>("Script",{
"SteelEngine",
"Script",
"Python"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(Script))
)
.Inheritance<IScript>("IScript")
.Constructor<>()
;
}

void Script::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(Script)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
}
}
