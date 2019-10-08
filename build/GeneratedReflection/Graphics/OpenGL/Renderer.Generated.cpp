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
;
}

void Renderer::Serialize(SteelEngine::HotReload::ISerializer* serializer)
{
SERIALIZE(Renderer::m_Window)
SERIALIZE(Renderer::m_Camera)
SERIALIZE(Renderer::m_QuadShader)
SERIALIZE(Renderer::m_QuadMesh)
SERIALIZE(Renderer::m_Models)
SERIALIZE(Renderer::m_Lights)
SERIALIZE(Renderer::m_Mesh)
SERIALIZE(Renderer::m_Mesh2)
SERIALIZE(Renderer::m_Mesh3)
SERIALIZE(Renderer::m_Texture)
SERIALIZE(Renderer::m_NormalMapTexture)
SERIALIZE(Renderer::m_DispMapTexture)
SERIALIZE(Renderer::m_Trans)
SERIALIZE(Renderer::m_Trans2)
SERIALIZE(Renderer::m_Trans3)
SERIALIZE(Renderer::m_G_Shader)
SERIALIZE(Renderer::m_PositionTexture)
SERIALIZE(Renderer::m_NormalTexture)
SERIALIZE(Renderer::m_AlbedoTexture)
SERIALIZE(Renderer::m_ShadowTexture)
SERIALIZE(Renderer::m_G_Buffer)
SERIALIZE(Renderer::m_ShadowShader)
SERIALIZE(Renderer::m_ShadowCamera)
SERIALIZE(Renderer::m_SpotLight)
SERIALIZE(Renderer::m_SpotRotation)
SERIALIZE(Renderer::m_DirectionalLight)
SERIALIZE(Renderer::m_DirectionalLightTransform)
SERIALIZE(Renderer::m_Counter)
SERIALIZE(Renderer::m_Keys)
SERIALIZE(Renderer::m_RotateCamera)
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
