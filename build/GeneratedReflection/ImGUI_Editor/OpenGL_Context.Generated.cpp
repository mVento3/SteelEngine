#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/ImGUI_Editor/OpenGL_Context.Generated.h"
#include "ImGUI_Editor/OpenGL_Context.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<OpenGL_Context>("OpenGL_Context",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(OpenGL_Context))
)
.Inheritance<IContext>("IContext")
.Constructor<>()
.Method("GetContext", &OpenGL_Context::GetContext)
.Method("MakeCurrent", &OpenGL_Context::MakeCurrent)
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
