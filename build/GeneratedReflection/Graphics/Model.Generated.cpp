#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Graphics/Model.h"
#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Graphics/Model.Generated.h"

namespace SteelEngine {
namespace Graphics {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<Model>("Model",{
"SteelEngine",
"Graphics"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(Model))
)
.Constructor<const char*>()
.Inheritance<IModel>("IModel")
;
}
#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(Model)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR(const char*)
};

return result;
}
#endif
}
}
