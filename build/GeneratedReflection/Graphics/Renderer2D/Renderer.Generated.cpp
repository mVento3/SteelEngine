#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Graphics/Renderer2D/Renderer.h"
#include "D:\Projects\C++\SteelEngine\build/GeneratedReflection/Graphics/Renderer2D/Renderer.Generated.h"

namespace SteelEngine {
namespace Graphics {
namespace Renderer2D {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<Renderer>("Renderer",{
"SteelEngine",
"Graphics",
"Renderer2D"
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
SERIALIZE(Renderer::m_FinalTexture)
SERIALIZE(Renderer::m_G_Framebuffer)
SERIALIZE(Renderer::m_VAO)
SERIALIZE(Renderer::m_VBO)
SERIALIZE(Renderer::m_IBO)
SERIALIZE(Renderer::m_Uniforms)
SERIALIZE(Renderer::m_Vertices)
SERIALIZE(Renderer::m_Buffer)
SERIALIZE(Renderer::m_DrawIndexCount)
SERIALIZE(Renderer::m_QuadVertexPositions)
SERIALIZE(Renderer::m_Shader)
SERIALIZE(Renderer::m_Projection)
SERIALIZE(Renderer::m_Model)
SERIALIZE(Renderer::m_View)
SERIALIZE(Renderer::m_CameraPosition)
SERIALIZE(Renderer::m_Keys)
SERIALIZE(Renderer::m_Scale)
SERIALIZE(Renderer::m_Width)
SERIALIZE(Renderer::m_Height)
SERIALIZE(Renderer::m_TestTexture1)
SERIALIZE(Renderer::m_TestTexture2)
SERIALIZE(Renderer::m_DeltaTimeVariant)
SERIALIZE(Renderer::test)
SERIALIZE(Renderer::m_DrawCalls)
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
