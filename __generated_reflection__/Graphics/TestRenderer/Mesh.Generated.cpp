#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Graphics/TestRenderer/Mesh.h"
#include "D:\Projects\C++\SteelEngine\__generated_reflection__/Graphics/TestRenderer/Mesh.Generated.h"

namespace SteelEngine {
namespace Graphics {
namespace TestRenderer {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<Mesh>("Mesh",{
"SteelEngine",
"Graphics",
"TestRenderer"
}
)
(
SteelEngine::Reflection::MetaData(IRenderer::RENDERER , "SteelEngine::Graphics::TestRenderer::Renderer"),
SteelEngine::Reflection::MetaData("sizeof", sizeof(Mesh))
)
.RegisterConstructor<>()
.RegisterInheritance<IMesh>("IMesh")
;
}
}
}
}
