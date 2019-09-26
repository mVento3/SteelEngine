#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Window/OpenGL_Window.Generated.h"
#include "Window/OpenGL_Window.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<OpenGL_Window>("OpenGL_Window",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(OpenGL_Window))
)
.Inheritance<IWindow>("IWindow")
.Constructor<>()
(
)
.Method("SetProcessEventsCallback", &OpenGL_Window::SetProcessEventsCallback)
(
)
.Method("GetContext", &OpenGL_Window::GetContext)
(
)
.Method("MakeCurrent", &OpenGL_Window::MakeCurrent)
(
)
;
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(void* typeInfo)
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
