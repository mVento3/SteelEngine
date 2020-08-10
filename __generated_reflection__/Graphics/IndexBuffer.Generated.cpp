#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/IndexBuffer.Generated.h"

namespace SteelEngine {
namespace Graphics {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<IndexBuffer>("IndexBuffer",{
"SteelEngine",
"Graphics"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(IndexBuffer))
)
.RegisterMethod("Create", { SteelEngine::Reflection::FucntionArgument<Type::uint32*>("indices"),
SteelEngine::Reflection::FucntionArgument<Type::uint32>("count") }, &IndexBuffer::Create)
;
}
}
}
