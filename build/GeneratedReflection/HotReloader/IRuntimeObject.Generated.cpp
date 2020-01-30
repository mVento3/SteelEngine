#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/HotReloader/IRuntimeObject.Generated.h"
#include "HotReloader/IRuntimeObject.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

namespace SteelEngine {
namespace HotReloader {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<IRuntimeObject>("IRuntimeObject",{
"SteelEngine",
"HotReloader"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(IRuntimeObject))
)
.Property("m_ObjectID", &IRuntimeObject::m_ObjectID)
.Property("m_TypeID", &IRuntimeObject::m_TypeID)
.Property("m_ConstructorID", &IRuntimeObject::m_ConstructorID)
.Property("m_Object", &IRuntimeObject::m_Object)
;
}

}
}
