#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Graphics/OpenGL/OpenGL_Renderer.Generated.h"
#include "Graphics/OpenGL/OpenGL_Renderer.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {
namespace Graphics {
namespace OpenGL {
REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<OpenGL_Renderer>("OpenGL_Renderer",{
"SteelEngine",
"Graphics",
"OpenGL"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::RUNTIME_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(OpenGL_Renderer))
)
.Inheritance<IRenderer>("IRenderer")
.Constructor<IWindow*>()
(
)
;
}

void OpenGL_Renderer::Serialize(SteelEngine::HotReload::ISerializer* serializer)
{
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(void* typeInfo)
{
DECLARE_TYPE_INFO(OpenGL_Renderer)
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
