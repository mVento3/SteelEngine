#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Graphics/TestDOD/Renderer.h"
#include "D:\Projects\C++\SteelEngine\__generated_reflection__/Graphics/TestDOD/Renderer.Generated.h"

namespace SteelEngine {
namespace Graphics {
namespace TestDOD {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<Renderer>("Renderer",{
"SteelEngine",
"Graphics",
"TestDOD"
}
)
(
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::RUNTIME_SERIALIZE, true),
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS, true),
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::HOT_RELOAD, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(Renderer))
)
.RegisterConstructor<>()
.RegisterInheritance<IRenderer>("IRenderer")
.RegisterInheritance<EventObserver>("EventObserver")
.RegisterMethod("Cast_IRenderer", &Renderer::Cast_IRenderer)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
.RegisterMethod("Cast_EventObserver", &Renderer::Cast_EventObserver)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
.RegisterMethod("Serialize", &Renderer::Serialize)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::SERIALIZE_FUNCTION, true)
)
;
}
void Renderer::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
SERIALIZE(Renderer::m_Window)
SERIALIZE(Renderer::m_Scene)
SERIALIZE(Renderer::m_RenderContext)
SERIALIZE(Renderer::m_GFX)
SERIALIZE(Renderer::m_CommandBuffer)
SERIALIZE(Renderer::texture)
SERIALIZE(Renderer::shader)
SERIALIZE(Renderer::resultTexture)
SERIALIZE(Renderer::result)
SERIALIZE(Renderer::vb)
SERIALIZE(Renderer::color)
SERIALIZE(Renderer::brick)
}
#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(Renderer)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
}
}
