#include "RuntimeReflection/ReflectionGeneratorMacros.h"

#define GENERATED_BODY \
public:\
VisualScript::IPin* Cast_IPin()\
{\
return static_cast<OutputClassPin*>(this);\
}\
VisualScript::IPinClassFunction* Cast_IPinClassFunction()\
{\
return static_cast<OutputClassPin*>(this);\
}\
JsonSerialization* Cast_JsonSerialization()\
{\
return static_cast<OutputClassPin*>(this);\
}
