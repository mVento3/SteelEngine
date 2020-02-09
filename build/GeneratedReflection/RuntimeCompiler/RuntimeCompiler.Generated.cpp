#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "RuntimeCompiler/RuntimeCompiler.h"
#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/RuntimeCompiler/RuntimeCompiler.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<RuntimeCompiler>("RuntimeCompiler",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(RuntimeCompiler))
)
.Constructor<>()
.Constructor<IPythonProcess*>()
.Inheritance<IRuntimeCompiler>("IRuntimeCompiler")
;
}
void RuntimeCompiler::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
}
#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(RuntimeCompiler)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
COMPARE_CONSTRUCTOR(IPythonProcess*)
};

return result;
}
#endif
}
