#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Editor/ImGUI/OpenGL_Context.h"
#include "D:\Projects\C++\SteelEngine\build/GeneratedReflection/Editor/ImGUI/OpenGL_Context.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<OpenGL_Context>("OpenGL_Context",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::HOT_RELOAD, true),
SteelEngine::Reflection::MetaData(Editor::ImGUI::ImGUI_Editor::API::CONTEXT_API , Graphics::IRenderer::API::OPENGL),
SteelEngine::Reflection::MetaData("sizeof", sizeof(OpenGL_Context))
)
.RegisterConstructor<>()
.RegisterInheritance<IContext>("IContext")
.RegisterMethod("GetContext", &OpenGL_Context::GetContext)
.RegisterMethod("MakeCurrent", &OpenGL_Context::MakeCurrent)
.RegisterMethod("Serialize", &OpenGL_Context::Serialize)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::SERIALIZE_FUNCTION, true)
)
;
}
void OpenGL_Context::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
}
#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(OpenGL_Context)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
