#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Graphics/TestRenderer/Framebuffer.h"
#include "D:\Projects\C++\SteelEngine\__generated_reflection__/Graphics/TestRenderer/Framebuffer.Generated.h"

namespace SteelEngine {
namespace Graphics {
namespace TestRenderer {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<Framebuffer>("Framebuffer",{
"SteelEngine",
"Graphics",
"TestRenderer"
}
)
(
SteelEngine::Reflection::MetaData(IRenderer::RENDERER , "SteelEngine::Graphics::TestRenderer::Renderer"),
SteelEngine::Reflection::MetaData("sizeof", sizeof(Framebuffer))
)
.RegisterConstructor<uint32_t, uint32_t, std::vector<FramebufferAttachment>>()
.RegisterInheritance<IFramebuffer>("IFramebuffer")
;
}
}
}
}
