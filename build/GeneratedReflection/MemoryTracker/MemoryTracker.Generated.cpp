#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/MemoryTracker/MemoryTracker.Generated.h"
#include "MemoryTracker/MemoryTracker.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

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
.Inheritance<IMemoryTracker>("IMemoryTracker")
.Constructor<Memory::Allocator*>()
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
