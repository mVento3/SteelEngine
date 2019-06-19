#include "RuntimeCompiler.Generated.h"
#include "../include\RuntimeCompiler\RuntimeCompiler.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<SteelEngine::RuntimeCompiler>("RuntimeCompiler")
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(SteelEngine::RuntimeCompiler))
)
.Inheritance("Interface::IRuntimeCompiler")
.Constructor<>()
(
)
.Property("m_IsCompileComplete", &SteelEngine::RuntimeCompiler::m_IsCompileComplete)
(
)
.Property("m_IsSwapComplete", &SteelEngine::RuntimeCompiler::m_IsSwapComplete)
(
)
.Property("m_Error", &SteelEngine::RuntimeCompiler::m_Error)
(
)
.Property("m_CmdProcessOutputRead", &SteelEngine::RuntimeCompiler::m_CmdProcessOutputRead)
(
)
.Property("m_CmdProcessInputWrite", &SteelEngine::RuntimeCompiler::m_CmdProcessInputWrite)
(
)
.Property("m_CmdProcessInfo", &SteelEngine::RuntimeCompiler::m_CmdProcessInfo)
(
)
;
}

void SteelEngine::RuntimeCompiler::Serialize(SteelEngine::Interface::ISerializer* serializer)
{
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* GetPerModuleInterface(void* typeInfo)
{
DECLARE_TYPE_INFO(SteelEngine::RuntimeCompiler)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
