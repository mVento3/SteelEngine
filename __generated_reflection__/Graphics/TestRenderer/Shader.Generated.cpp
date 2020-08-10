#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Graphics/TestRenderer/Shader.h"
#include "Graphics/TestRenderer/Shader.Generated.h"

namespace SteelEngine {
namespace Graphics {
namespace TestRenderer {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<Shader>("Shader",{
"SteelEngine",
"Graphics",
"TestRenderer"
}
)
(
SteelEngine::Reflection::MetaData(IRenderer::RENDERER , "SteelEngine::Graphics::TestRenderer::Renderer"),
SteelEngine::Reflection::MetaData("sizeof", sizeof(Shader))
)
.RegisterConstructor<const char*, const char*>()
.RegisterInheritance<IShader>("IShader")
;
}
}
}
}
