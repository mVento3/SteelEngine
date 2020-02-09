#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Window/OpenGL_Window.h"
#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Window/OpenGL_Window.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<OpenGL_Window>("OpenGL_Window",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(OpenGL_Window))
)
.Constructor<>()
.Inheritance<IWindow>("IWindow")
.Method("SetProcessEventsCallback", &OpenGL_Window::SetProcessEventsCallback)
.Method("GetContext", &OpenGL_Window::GetContext)
.Method("MakeCurrent", &OpenGL_Window::MakeCurrent)
;
}
#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(OpenGL_Window)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
