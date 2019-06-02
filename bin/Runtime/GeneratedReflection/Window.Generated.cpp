#include "Window.Generated.h"
#include "../include\Window\Window.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<SteelEngine::Window>("Window")
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::SE_NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(SteelEngine::Window))
)
.Inheritance("Interface::IWindow")
.Constructor<>()
(
)
;
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* GetPerModuleInterface(void* typeInfo)
{
DECLARE_TYPE_INFO(SteelEngine::Window)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
