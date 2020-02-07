#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "MemoryTracker/MemoryTracker.h"
#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/MemoryTracker/MemoryTracker.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<MemoryTracker>("MemoryTracker",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(MemoryTracker))
)
.Constructor<Memory::Allocator*>()
.Inheritance<IMemoryTracker>("IMemoryTracker")
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
