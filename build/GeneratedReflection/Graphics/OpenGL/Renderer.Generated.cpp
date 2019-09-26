#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Graphics/OpenGL/Renderer.Generated.h"
#include "Graphics/OpenGL/Renderer.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {
namespace Graphics {
namespace OpenGL {
REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<Renderer>("Renderer",{
"SteelEngine",
"Graphics",
"OpenGL"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::RUNTIME_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(Renderer))
)
.Inheritance<IRenderer>("IRenderer")
.Constructor<IWindow*>()
(
)
.Method("GlewInit", &Renderer::GlewInit)
(
)
;
}

void Renderer::Serialize(SteelEngine::HotReload::ISerializer* serializer)
{
SERIALIZE(Renderer::m_Window)
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(void* typeInfo)
{
DECLARE_TYPE_INFO(Renderer)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR(IWindow*)
};

return result;
}
#endif
}
}
}
