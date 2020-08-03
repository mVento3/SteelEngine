#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "MemoryTracker/MemoryTracker.h"
#include "D:\Projects\C++\SteelEngine\__generated_reflection__/MemoryTracker/MemoryTracker.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<MemoryTracker>("MemoryTracker",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::HOT_RELOAD, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(MemoryTracker))
)
.RegisterConstructor<Memory::Allocator*>()
.RegisterInheritance<IMemoryTracker>("IMemoryTracker")
.RegisterMethod("Serialize", &MemoryTracker::Serialize)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::SERIALIZE_FUNCTION, true)
)
;
}
void MemoryTracker::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
}
#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(MemoryTracker)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR(Memory::Allocator*)
};

return result;
}
#endif
}
