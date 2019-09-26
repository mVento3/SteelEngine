#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/RuntimeCompiler/IRuntimeObject.Generated.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {
namespace HotReload {
REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<IRuntimeObject>("IRuntimeObject",{
"SteelEngine",
"HotReload"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(IRuntimeObject))
)
.Property("m_ObjectID", &IRuntimeObject::m_ObjectID)
(
)
.Property("m_TypeID", &IRuntimeObject::m_TypeID)
(
)
.Property("m_ConstructorID", &IRuntimeObject::m_ConstructorID)
(
)
.Property("m_Object", &IRuntimeObject::m_Object)
(
)
;
}

}
}
