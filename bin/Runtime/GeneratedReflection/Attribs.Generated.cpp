#include "Attribs.Generated.h"
#include "../include\Core\Attribs.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<SteelEngine::Attribs>("Attribs")
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(SteelEngine::Attribs))
)
.Constructor<>()
(
)
.Enum<SteelEngine::Attribs::TestE>("TestE")
(
)
.Values 
(
SteelEngine::ReflectionValue("TEST1", SteelEngine::Attribs::TestE::TEST1)
(
)
,
SteelEngine::ReflectionValue("TEST2", SteelEngine::Attribs::TestE::TEST2)
(
)

)
.Enum<SteelEngine::Attribs::TestE2>("TestE2")
(
)
.Values
(
SteelEngine::ReflectionValue("TESTE1", SteelEngine::Attribs::TestE2::TESTE1)
(
)
,
SteelEngine::ReflectionValue("TESTE2", SteelEngine::Attribs::TestE2::TESTE2)
(
)

)
;
}

void SteelEngine::Attribs::Serialize(SteelEngine::Interface::ISerializer* serializer)
{
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* GetPerModuleInterface(void* typeInfo)
{
DECLARE_TYPE_INFO(SteelEngine::Attribs)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
