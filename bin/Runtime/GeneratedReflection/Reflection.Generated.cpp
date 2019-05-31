#include "Reflection.Generated.h"
#include "../include\Utils\Reflection.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<::index_tuple>("index_tuple")
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(::index_tuple))
)
;
}

void ::index_tuple::Serialize(SteelEngine::Interface::ISerializer* serializer)
{
}

