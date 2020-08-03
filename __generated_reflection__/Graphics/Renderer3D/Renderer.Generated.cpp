#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Graphics/Renderer3D/Renderer.h"
#include "D:\Projects\C++\SteelEngine\__generated_reflection__/Graphics/Renderer3D/Renderer.Generated.h"

namespace SteelEngine {
namespace Graphics {
namespace Renderer3D {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<Renderer>("Renderer",{
"SteelEngine",
"Graphics",
"Renderer3D"
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
SERIALIZE(Renderer::m_FinalTexture)
SERIALIZE(Renderer::m_GBuffer)
SERIALIZE(Renderer::m_Window)
SERIALIZE(Renderer::m_GShader)
SERIALIZE(Renderer::m_Camera)
SERIALIZE(Renderer::m_Uniforms)
SERIALIZE(Renderer::m_SomeTexture)
SERIALIZE(Renderer::m_Keys)
SERIALIZE(Renderer::m_Controlls)
SERIALIZE(Renderer::m_RotateCamera)
SERIALIZE(Renderer::m_Scene)
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
