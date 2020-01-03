#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/VirtualProject/VirtualProjectVisualizer.Generated.h"
#include "VirtualProject/VirtualProjectVisualizer.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<VirtualProjectVisualizer>("VirtualProjectVisualizer",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(VirtualProjectVisualizer))
)
.Inheritance<IVirtualProjectVisualizer>("IVirtualProjectVisualizer")
.Constructor<VirtualProject**>()
;
}

void VirtualProjectVisualizer::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(VirtualProjectVisualizer)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR(VirtualProject**)
};

return result;
}
#endif
}
