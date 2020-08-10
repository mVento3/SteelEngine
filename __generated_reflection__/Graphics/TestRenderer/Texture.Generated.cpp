#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Graphics/TestRenderer/Texture.h"
#include "Graphics/TestRenderer/Texture.Generated.h"

namespace SteelEngine {
namespace Graphics {
namespace TestRenderer {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<Texture>("Texture",{
"SteelEngine",
"Graphics",
"TestRenderer"
}
)
(
SteelEngine::Reflection::MetaData(IRenderer::RENDERER , "SteelEngine::Graphics::TestRenderer::Renderer"),
SteelEngine::Reflection::MetaData("sizeof", sizeof(Texture))
)
.RegisterConstructor<>()
.RegisterConstructor<const char*>()
.RegisterConstructor<InternalFormat, Format, DataType, Filter, bool>()
.RegisterConstructor<uint32_t, uint32_t, InternalFormat, Format, DataType, Filter, bool>()
.RegisterInheritance<ITexture>("ITexture")
;
}
}
}
}
