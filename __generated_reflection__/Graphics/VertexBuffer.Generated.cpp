#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/VertexBuffer.Generated.h"

namespace SteelEngine {
namespace Graphics {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<VertexBuffer>("VertexBuffer",{
"SteelEngine",
"Graphics"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(VertexBuffer))
)
.RegisterMethod("Create", { SteelEngine::Reflection::FucntionArgument<float*>("vertices"),
SteelEngine::Reflection::FucntionArgument<Type::uint32>("count") }, &VertexBuffer::Create)
;
}
}
}
